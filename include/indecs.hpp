#pragma once

#include <unordered_map>
#include <vector>

struct Position {
    float x, y;
};

struct Velocity {
    float vx, vy;
};

struct Acceleration {
    float ax, ay;
};

struct Health {
    float hp;
};

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

  public:
    // using ComponentSet = std::tuple<Ts...>;

    template <typename... Us>
    static constexpr bool matches = sizeof...(Us) == sizeof...(Components) &&
                                    (IsInPack<Us, Components...> && ...);

    template <typename F> void forEachVector(F &&func) {
        std::apply(std::forward<F>(func), components);
    }

    [[nodiscard]]
    auto size() const -> size_t {
        return entities.size();
    }

    /// pushes an entity and the archetype data into the archetype
    void pushBack(Entity entity, Components &...data) {
        entities.push_back(entity);

        // using ComponentVec<T> = std::vector<std::remove_cvref_t<T>>;
        (std::get<std::vector<std::remove_cvref_t<Components>>>(components)
             .pushBack(std::forward(data)),
         ...);
    };

    void swapAndDecrSize(size_t idx) {
        if (idx != entities.size() - 1) {
            entities[idx] = std::move(entities.back());

            forEachVector([&](auto &...vecs) -> void {
                ((vecs[idx] = std::move(vecs.back())), ...);
            });
        }

        entities.pop_back();

        forEachVector([&](auto &...vecs) -> void { ((vecs.pop_back()), ...); });
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
