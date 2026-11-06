#include <catch2/catch_test_macros.hpp>

#include <indecs.hpp>

#include "components.hpp"

using idcs::Archetype;
using idcs::Entity;

using Mover = Archetype<Position, Velocity>;
using Physical = Archetype<Position, Velocity, Acceleration>;
using Fighter = Archetype<Health, Attack>;
using Prop = Archetype<Position, Name>;

using GameWorld = idcs::World<Mover, Physical, Fighter, Prop>;

TEST_CASE("World archetypeIdx", "[world]") {
    GameWorld world;

    CHECK(world.archetypeIdx<Mover>() == 0);
    CHECK(world.archetypeIdx<Physical>() == 1);
    CHECK(world.archetypeIdx<Fighter>() == 2);
    CHECK(world.archetypeIdx<Prop>() == 3);
}

TEST_CASE("World spawn same archetype", "[world]") {
    GameWorld world;

    Entity ball = world.spawn<Mover>({Position{.x = 1, .y = 2}, Velocity{.vx = 3, .vy = 4}});
    Entity square = world.spawn<Mover>({Position{.x = 2, .y = 3}, Velocity{.vx = 4, .vy = 5}});
    Entity triangle = world.spawn<Mover>({Position{.x = 3, .y = 4}, Velocity{.vx = 5, .vy = 6}});

    CHECK(world.entityExists(ball));
    CHECK(world.entityExists(square));
    CHECK(world.entityExists(triangle));

    CHECK(world.entityArchetypeIdx(ball) == 0);
    CHECK(world.entityArchetypeIdx(square) == 0);
    CHECK(world.entityArchetypeIdx(triangle) == 0);

    CHECK(world.getIdx(ball) == 0);
    CHECK(world.getArchetype<Mover>().get<Position>(0) == Position{1, 2});
    CHECK(world.getArchetype<Mover>().get<Velocity>(0) == Velocity{3, 4});

    CHECK(world.getIdx(square) == 1);
    CHECK(world.getArchetype<Mover>().get<Position>(1) == Position{2, 3});
    CHECK(world.getArchetype<Mover>().get<Velocity>(1) == Velocity{4, 5});

    CHECK(world.getIdx(triangle) == 2);
    CHECK(world.getArchetype<Mover>().get<Position>(2) == Position{3, 4});
    CHECK(world.getArchetype<Mover>().get<Velocity>(2) == Velocity{5, 6});
}

TEST_CASE("World spawn different archetype", "[world]") {
    GameWorld world;

    Entity ball = world.spawn<Mover>({
        Position{.x = 1, .y = 2},
        Velocity{.vx = 3, .vy = 4},
    });
    Entity bullet = world.spawn<Physical>({
        Position{.x = 2, .y = 3},
        Velocity{.vx = 4, .vy = 5},
        Acceleration{.ax = 6, .ay = 7},
    });
    Entity player = world.spawn<Fighter>({
        Health{100},
        Attack{50},
    });
    Entity tree = world.spawn<Prop>({
        Position{.x = 100, .y = 500},
        Name{"biggie trees"},
    });

    CHECK(world.entityExists(ball));
    CHECK(world.entityExists(bullet));
    CHECK(world.entityExists(player));
    CHECK(world.entityExists(tree));

    CHECK(world.entityArchetypeIdx(ball) == 0);
    CHECK(world.entityArchetypeIdx(bullet) == 1);
    CHECK(world.entityArchetypeIdx(player) == 2);
    CHECK(world.entityArchetypeIdx(tree) == 3);

    CHECK(world.getIdx(ball) == 0);
    CHECK(world.getArchetype<Mover>().get<Position>(0) == Position{1, 2});
    CHECK(world.getArchetype<Mover>().get<Velocity>(0) == Velocity{3, 4});

    CHECK(world.getIdx(bullet) == 0);
    CHECK(world.getArchetype<Physical>().get<Position>(0) == Position{2, 3});
    CHECK(world.getArchetype<Physical>().get<Velocity>(0) == Velocity{4, 5});
    CHECK(world.getArchetype<Physical>().get<Acceleration>(0) == Acceleration{6, 7});

    CHECK(world.getIdx(player) == 0);
    CHECK(world.getArchetype<Fighter>().get<Health>(0) == Health{100});
    CHECK(world.getArchetype<Fighter>().get<Attack>(0) == Attack{50});

    CHECK(world.getIdx(tree) == 0);
    CHECK(world.getArchetype<Prop>().get<Position>(0) == Position{100, 500});
    CHECK(world.getArchetype<Prop>().get<Name>(0) == Name{"biggie trees"});
}
