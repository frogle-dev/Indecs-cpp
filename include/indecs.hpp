#pragma once

#include <cassert>
#include <unordered_map>
#include <vector>

namespace Indecs {
using Entity = size_t;

template <typename T, typename... Ts>
concept IsInPack = (std::is_same_v<T, Ts> || ...);

// TODO: NO DUPLICATE TYPES IN ARCHETYPE
template <typename... Components> class Archetype {
    std::vector<Entity> entities;
    std::tuple<std::vector<Components>...> components;

    /// private since all the vectors in 'components' have to be the same size
    /// and this is the archetype's responsibility
    template <IsInPack<Components...> T>
    constexpr auto getComponentVec() -> std::vector<T> & {
        return std::get<std::vector<T>>(components);
    }

    template <typename F> void forEachVector(F &&func) {
        std::apply(
            [&](auto &...vecs) -> void {
                ((std::forward<F>(func)(vecs)), ...);
            },
            components);
    }

    template <typename F> void forEachVector(F &&func) const {
        std::apply(
            [&](auto &...vecs) -> void {
                ((std::forward<F>(func)(vecs)), ...);
            },
            components);
    }

  public:
    // using ComponentSet = std::tuple<Ts...>;

    template <typename... Us>
    static constexpr bool matches = sizeof...(Us) == sizeof...(Components) &&
                                    (IsInPack<Us, Components...> && ...);

    void assertSameSizes() const {
#ifndef NDEBUG
        forEachVector(
            [&](auto &vec) -> void { assert(vec.size() == entities.size()); });
#endif
    }

    [[nodiscard]]
    auto size() const -> size_t {
        return entities.size();
    }

    /// pushes an entity and the archetype data into the archetype
    void pushBack(Entity entity, Components... data) {
        entities.push_back(entity);

        (getComponentVec<Components>().push_back(std::move(data)), ...);

        assertSameSizes();
    }

    void swapAndDecrSize(size_t idx) {
        if (idx != entities.size() - 1) {
            entities[idx] = std::move(entities.back());

            forEachVector(
                [&](auto &vec) -> void { (vec[idx] = std::move(vec.back())); });
        }

        entities.pop_back();

        forEachVector([&](auto &vec) -> void { ((vec.pop_back())); });
    }
};

struct EntityLocation {
    size_t archetype;
    size_t idx;
};

template <typename... Archetypes> class World {
    std::tuple<Archetypes...> archetypes;

    std::unordered_map<Entity, EntityLocation> entityLocations;

    Entity nextEntityId = 0;

  public:
    World() = default;
    ~World() = default;

    template <typename... Components>
    static constexpr auto archetypeIdx() -> size_t {
        constexpr std::array results{
            Archetypes::template matches<
                std::remove_cvref_t<Components>...>()...,
        };

        for (size_t i = 0; i < sizeof...(Archetypes); i++) {
            if (results[i]) {
                return i;
            }
        }

        return sizeof...(Archetypes);
    }

    template <typename... Components>
    auto spawn(Components &&...components) -> Entity {
        constexpr size_t archIdx = archetypeIdx<Components...>();
        static_assert(archIdx < sizeof...(Archetypes),
                      "no archetype matches the given components");

        auto &archetype = std::get<archIdx>(archetypes);
        const Entity entityId = nextEntityId++;
        const size_t idx = archetype.size();

        archetype.pushBack(entityId, std::forward<Components>(components)...);
        entityLocations[entityId] = EntityLocation{archetypeIdx, idx};

        return entityId;
    }
};
} // namespace Indecs
