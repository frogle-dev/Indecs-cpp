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

template <typename... Args> class Archetype {
    std::vector<Entity> entities;
    std::tuple<std::vector<Args>...> components;

  public:
    using ComponentSet = std::tuple<Args...>;

    template <typename T>
    static constexpr bool has = (std::is_same_v<T, Args> || ...);

    template <typename... Ts> static constexpr auto matches() -> bool {
        return sizeof...(Ts) == sizeof...(Args) && (has<Ts> && ...);
    }

    template <typename F> void forEachVector(F &&func) {
        std::apply(std::forward<F>(func), components);
    }

    void pushBack(Args... data, Entity newEntityId) {
        entities.push_back(newEntityId);

        forEachVector([&](auto &...vecs) -> void {
            (vecs.push_back(std::move(data)), ...);
        });
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

  public:
    World() = default;
    ~World() = default;

    template <typename... Components> constexpr size_t archetypeIdx() {}

    template <typename... Components> Entity spawn(Components &&...components) {
        // remove_cvref_t removes things like const and references and
        // leaves just the type
        using Set = std::tuple<std::remove_cvref_t<Components>...>;
        if constexpr (std::is_same_v<
                          Set, std::tuple<decltype(player)::ComponentSet>>) {
        }
        if constexpr (std::is_same_v<
                          Set, std::tuple<decltype(enemy)::ComponentSet>>) {
        }
        if constexpr (std::is_same_v<Set, std::tuple<decltype(tree)>>) {
        }
    }
};
} // namespace Indecs
