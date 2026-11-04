#include <catch2/catch_test_macros.hpp>

#include <indecs.hpp>

struct Position {
    float x, y;

    auto operator==(Position const &) const -> bool = default;
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

TEST_CASE("Archetype pushBack", "[archetype]") {
    auto player = Indecs::Archetype<Position, Velocity, Health>();
    player.pushBack(0, Position{5, 10}, Velocity{1, 2}, Health{50});

    REQUIRE(player.size() == 1);
    REQUIRE(player.get<Position>(0).x == 5);
    REQUIRE(player.get<Position>(0).y == 10);
    REQUIRE(player.get<Position>(0).x == 5);
    REQUIRE(player.get<Position>(0).y == 10);
}
