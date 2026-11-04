#include <indecs.hpp>

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

auto main() -> int {
    auto player = Indecs::Archetype<Position, Velocity, Health>();
    player.pushBack(0, Position{0, 0}, Velocity{0, 0}, Health{50});
}
