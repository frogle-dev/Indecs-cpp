#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace idcs {
using Entity = size_t;

// template <typename T> struct Entity {
//     using Archetype = T;
//
//     size_t id;
// };

template <typename T, typename... Ts>
concept IsInPack = (std::is_same_v<T, Ts> || ...);

// TODO: NO DUPLICATE TYPES IN ARCHETYPE
template <typename... Components> class Archetype {
    std::vector<Entity> entities;
    std::tuple<std::vector<Components>...> components;

    /// private since all the vectors in 'components' have to be the same size
    /// and this is the archetype's responsibility
    template <IsInPack<Components...> T> constexpr auto getComponentVec() -> std::vector<T> & {
        return std::get<std::vector<T>>(components);
    }

    template <typename F> void forEachVector(F &&func) {
        std::apply([&](auto &...vecs) -> void { ((std::forward<F>(func)(vecs)), ...); },
                   components);
    }

    template <typename F> void forEachVector(F &&func) const {
        std::apply([&](auto &...vecs) -> void { ((std::forward<F>(func)(vecs)), ...); },
                   components);
    }

  public:
    using ComponentSet = std::tuple<Components...>;

    [[nodiscard]] auto getEntity(size_t idx) const -> Entity { return entities[idx]; }

    template <IsInPack<Components...> T> [[nodiscard]] auto get(size_t idx) const -> T const & {
        return std::get<std::vector<T>>(components)[idx];
    }

    void assertSameSizes() const {
#ifndef NDEBUG
        forEachVector([&](auto &vec) -> void { assert(vec.size() == entities.size()); });
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

    void swapAndPop(size_t idx) {
        if (idx != entities.size() - 1) {
            entities[idx] = entities.back();

            forEachVector([&](auto &vec) -> void { (vec[idx] = std::move(vec.back())); });
        }

        entities.pop_back();

        forEachVector([&](auto &vec) -> void { ((vec.pop_back())); });

        assertSameSizes();
    }
};

struct EntityLocation {
    size_t archetype;
    size_t idx;
};

template <typename T> struct IsArchetypeImpl : std::false_type {};
template <typename... Cs> struct IsArchetypeImpl<Archetype<Cs...>> : std::true_type {};

template <typename T>
concept IsArchetype = IsArchetypeImpl<T>::value;

template <IsArchetype... Archetypes> class World {
    std::tuple<Archetypes...> archetypes;

    std::unordered_map<Entity, EntityLocation> entityLocations;

    /// TODO: potentially a list of despawned entityIds to reuse
    Entity nextEntityId = 0;

    template <IsArchetype Target>
    static constexpr bool archetypeInWorld = (std::is_same_v<Target, Archetypes> || ...);

    template <IsArchetype Target>
        requires(archetypeInWorld<Target>)
    [[nodiscard]] auto getArchetypeRef() -> Target & {
        return std::get<Target>(archetypes);
    }

  public:
    World() = default;
    ~World() = default;

    template <IsArchetype Target>
        requires(archetypeInWorld<Target>)
    [[nodiscard]] auto getArchetype() const -> Target const & {
        return std::get<Target>(archetypes);
    }

    template <IsArchetype Target>
        requires(archetypeInWorld<Target>)
    static constexpr auto archetypeIdx() -> size_t {
        constexpr std::array matching{std::is_same_v<Target, Archetypes>...};

        for (size_t i = 0; i < sizeof...(Archetypes); i++) {
            if (matching[i]) {
                return i;
            }
        }

        std::unreachable();
    }

    // TODO: use std expected for entityExists
    [[nodiscard]] auto entityArchetypeIdx(Entity entity) const -> size_t {
        return entityLocations.at(entity).archetype;
    }

    template <IsArchetype Target>
        requires(archetypeInWorld<Target>)
    auto spawn(Target::ComponentSet components) -> Entity {
        auto &archetype = getArchetypeRef<Target>();

        Entity const entityId = nextEntityId++;
        std::apply(
            [&](auto &...components) -> void {
                archetype.pushBack(entityId, std::move(components)...);
            },
            components);

        size_t const idx = archetype.size() - 1;
        entityLocations[entityId] = EntityLocation{archetypeIdx<Target>(), idx};

        return entityId;
    }

    template <IsArchetype Target> void despawn(Entity entity) {
        auto loc = entityLocations.at(entity);

        auto &archetype = std::get<Target>(archetypes);
        archetype.swapAndPop(loc.idx);

        entityLocations.erase(entity);

        EntityLocation &swappedLoc = entityLocations.at(archetype.getEntity(loc.idx));
        swappedLoc.idx = loc.idx;
    }

    auto entityExists(Entity entity) const -> bool { return entityLocations.contains(entity); }
    auto getIdx(Entity entity) const -> size_t { return entityLocations.at(entity).idx; }
};
} // namespace idcs
