#include <catch2/catch_test_macros.hpp>

#include <indecs.hpp>

struct Position {
    float x, y;

    auto operator==(Position const &) const -> bool = default;
};

struct Velocity {
    float vx, vy;

    auto operator==(Velocity const &) const -> bool = default;
};

struct Acceleration {
    float ax, ay;

    auto operator==(Acceleration const &) const -> bool = default;
};

struct Health {
    float hp;

    auto operator==(Health const &) const -> bool = default;
};

TEST_CASE("Archetype 1 pushBack", "[archetype]") {
    Velocity vel = {
        .vx = 1,
        .vy = 2,
    };

    Acceleration acc = {
        .ax = 6,
        .ay = 7,
    };

    auto player = Indecs::Archetype<Position, Velocity, Acceleration, Health>();
    player.pushBack(5, Position{5, 10}, std::move(vel), acc, Health{50});

    REQUIRE(player.size() == 1);
    REQUIRE(player.getEntity(0) == 5);
    REQUIRE(player.get<Position>(0) == Position{5, 10});
    REQUIRE(player.get<Velocity>(0) == vel);
    REQUIRE(player.get<Acceleration>(0) == acc);
    REQUIRE(player.get<Health>(0).hp == 50);
}

TEST_CASE("Archetype 3 pushBack", "[archetype]") {
    auto player = Indecs::Archetype<Position, Velocity, Acceleration>();
    player.pushBack(2, Position{1, 2}, Velocity{3, 4}, Acceleration{5, 6});
    player.pushBack(1, Position{2, 3}, Velocity{4, 5}, Acceleration{6, 7});
    player.pushBack(0, Position{3, 4}, Velocity{5, 6}, Acceleration{7, 8});

    REQUIRE(player.size() == 3);
    REQUIRE(player.getEntity(0) == 2);
    REQUIRE(player.get<Position>(0) == Position{1, 2});
    REQUIRE(player.get<Velocity>(0) == Velocity{3, 4});
    REQUIRE(player.get<Acceleration>(0) == Acceleration{5, 6});

    REQUIRE(player.getEntity(1) == 1);
    REQUIRE(player.get<Position>(1) == Position{2, 3});
    REQUIRE(player.get<Velocity>(1) == Velocity{4, 5});
    REQUIRE(player.get<Acceleration>(1) == Acceleration{6, 7});

    REQUIRE(player.getEntity(2) == 0);
    REQUIRE(player.get<Position>(2) == Position{3, 4});
    REQUIRE(player.get<Velocity>(2) == Velocity{5, 6});
    REQUIRE(player.get<Acceleration>(2) == Acceleration{7, 8});
}
