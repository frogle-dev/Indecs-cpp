#include <catch2/catch_test_macros.hpp>

#include <indecs.hpp>

#include "components.hpp"

using idcs::Archetype;

TEST_CASE("World archetypeIdx", "[world]") {
    using Mover = Archetype<Position, Velocity>;
    using Physical = Archetype<Position, Velocity, Acceleration>;
    using Fighter = Archetype<Health, Attack>;
    using Prop = Archetype<Position, Name>;

    using GameWorld = idcs::World<Mover, Physical, Fighter, Prop>;

    GameWorld world;

    CHECK(world.archetypeIdx<Mover>() == 0);
    CHECK(world.archetypeIdx<Physical>() == 1);
    CHECK(world.archetypeIdx<Fighter>() == 2);
    CHECK(world.archetypeIdx<Prop>() == 3);
}

TEST_CASE("World spawn", "[world]") {
    using Mover = Archetype<Position, Velocity>;
    using Physical = Archetype<Position, Velocity, Acceleration>;
    using Fighter = Archetype<Health, Attack>;
    using Prop = Archetype<Position, Name>;

    using GameWorld = idcs::World<Mover, Physical, Fighter, Prop>;

    GameWorld world;

    idcs::Entity ball = world.spawn<Mover>({Position{.x = 1, .y = 2}, Velocity{.vx = 3, .vy = 4}});

    CHECK(world.entityExists(ball));
    CHECK(world.entityArchetypeIdx(ball) == 0);
}
