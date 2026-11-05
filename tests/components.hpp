#include <string>

struct Name {
    std::string name;
};

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

struct Attack {
    float atk;

    auto operator==(Attack const &) const -> bool = default;
};
