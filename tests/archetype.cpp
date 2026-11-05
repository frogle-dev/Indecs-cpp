#include <catch2/catch_test_macros.hpp>

#include "components.hpp"
#include <indecs.hpp>

TEST_CASE("Archetype 1 pushBack", "[archetype]") {
    Acceleration acc = {
        .ax = 6,
        .ay = 7,
    };

    Name name = {
        .name = "ooga booga",
    };

    auto player = idcs::Archetype<Position, Name, Acceleration, Health>();
    player.pushBack(5, Position{5, 10}, std::move(name), acc, Health{50});

    REQUIRE(player.size() == 1);

    CHECK(player.getEntity(0) == 5);
    CHECK(player.get<Position>(0) == Position{5, 10});
    CHECK(player.get<Name>(0).name == "ooga booga");
    CHECK(player.get<Acceleration>(0) == acc);
    CHECK(player.get<Health>(0).hp == 50);
}

TEST_CASE("Archetype 3 pushBack", "[archetype]") {
    auto player = idcs::Archetype<Position, Velocity, Acceleration>();
    player.pushBack(2, Position{1, 2}, Velocity{3, 4}, Acceleration{5, 6});
    player.pushBack(1, Position{2, 3}, Velocity{4, 5}, Acceleration{6, 7});
    player.pushBack(0, Position{3, 4}, Velocity{5, 6}, Acceleration{7, 8});

    REQUIRE(player.size() == 3);

    CHECK(player.getEntity(0) == 2);
    CHECK(player.get<Position>(0) == Position{1, 2});
    CHECK(player.get<Velocity>(0) == Velocity{3, 4});
    CHECK(player.get<Acceleration>(0) == Acceleration{5, 6});

    CHECK(player.getEntity(1) == 1);
    CHECK(player.get<Position>(1) == Position{2, 3});
    CHECK(player.get<Velocity>(1) == Velocity{4, 5});
    CHECK(player.get<Acceleration>(1) == Acceleration{6, 7});

    CHECK(player.getEntity(2) == 0);
    CHECK(player.get<Position>(2) == Position{3, 4});
    CHECK(player.get<Velocity>(2) == Velocity{5, 6});
    CHECK(player.get<Acceleration>(2) == Acceleration{7, 8});
}

TEST_CASE("Archetype swapAndPop first idx", "[archetype]") {
    auto player = idcs::Archetype<Position, Velocity, Acceleration>();
    player.pushBack(0, Position{1, 2}, Velocity{3, 4}, Acceleration{5, 6});
    player.pushBack(1, Position{2, 3}, Velocity{4, 5}, Acceleration{6, 7});
    player.pushBack(2, Position{3, 4}, Velocity{5, 6}, Acceleration{7, 8});

    player.swapAndPop(0);

    REQUIRE(player.size() == 2);

    // should be last element data, since swapped
    CHECK(player.getEntity(0) == 2);
    CHECK(player.get<Position>(0) == Position{3, 4});
    CHECK(player.get<Velocity>(0) == Velocity{5, 6});
    CHECK(player.get<Acceleration>(0) == Acceleration{7, 8});

    // idx 1 should still be the same
    CHECK(player.getEntity(1) == 1);
    CHECK(player.get<Position>(1) == Position{2, 3});
    CHECK(player.get<Velocity>(1) == Velocity{4, 5});
    CHECK(player.get<Acceleration>(1) == Acceleration{6, 7});
}

TEST_CASE("Archetype swapAndPop last idx", "[archetype]") {
    auto player = idcs::Archetype<Position, Velocity, Acceleration>();
    player.pushBack(0, Position{1, 2}, Velocity{3, 4}, Acceleration{5, 6});
    player.pushBack(1, Position{2, 3}, Velocity{4, 5}, Acceleration{6, 7});
    player.pushBack(2, Position{3, 4}, Velocity{5, 6}, Acceleration{7, 8});

    player.swapAndPop(2);

    REQUIRE(player.size() == 2);

    // since the lsat element is deleted, the first two should be the same
    CHECK(player.getEntity(0) == 0);
    CHECK(player.get<Position>(0) == Position{1, 2});
    CHECK(player.get<Velocity>(0) == Velocity{3, 4});
    CHECK(player.get<Acceleration>(0) == Acceleration{5, 6});

    CHECK(player.getEntity(1) == 1);
    CHECK(player.get<Position>(1) == Position{2, 3});
    CHECK(player.get<Velocity>(1) == Velocity{4, 5});
    CHECK(player.get<Acceleration>(1) == Acceleration{6, 7});
}

TEST_CASE("Archetype swapAndPop on size 1 Archetype", "[archetype]") {
    auto player = idcs::Archetype<Position, Velocity, Acceleration>();
    player.pushBack(5, Position{1, 2}, Velocity{3, 4}, Acceleration{5, 6});

    player.swapAndPop(0);

    REQUIRE(player.size() == 0);

    player.pushBack(9, {0, 1}, {2, 3}, {3, 4});

    // state should be valid after clearing the archetype
    CHECK(player.getEntity(0) == 9);
    CHECK(player.get<Position>(0) == Position{0, 1});
    CHECK(player.get<Velocity>(0) == Velocity{2, 3});
    CHECK(player.get<Acceleration>(0) == Acceleration{3, 4});
}

TEST_CASE("Archetype multiple swapAndPops", "[archetype]") {
    auto player = idcs::Archetype<Position, Velocity, Acceleration>();
    player.pushBack(0, Position{1, 2}, Velocity{3, 4}, Acceleration{5, 6});
    player.pushBack(1, Position{2, 3}, Velocity{4, 5}, Acceleration{6, 7});
    player.pushBack(2, Position{3, 4}, Velocity{5, 6}, Acceleration{7, 8});
    player.pushBack(3, Position{4, 5}, Velocity{6, 7}, Acceleration{8, 9});
    player.pushBack(4, Position{5, 6}, Velocity{7, 8}, Acceleration{9, 10});
    player.pushBack(5, Position{6, 7}, Velocity{8, 9}, Acceleration{10, 11});

    player.swapAndPop(1);
    player.swapAndPop(1);
    player.swapAndPop(3);

    REQUIRE(player.size() == 3);

    CHECK(player.getEntity(0) == 0);
    CHECK(player.get<Position>(0) == Position{1, 2});
    CHECK(player.get<Velocity>(0) == Velocity{3, 4});
    CHECK(player.get<Acceleration>(0) == Acceleration{5, 6});

    CHECK(player.getEntity(1) == 4);
    CHECK(player.get<Position>(1) == Position{5, 6});
    CHECK(player.get<Velocity>(1) == Velocity{7, 8});
    CHECK(player.get<Acceleration>(1) == Acceleration{9, 10});

    CHECK(player.getEntity(2) == 2);
    CHECK(player.get<Position>(2) == Position{3, 4});
    CHECK(player.get<Velocity>(2) == Velocity{5, 6});
    CHECK(player.get<Acceleration>(2) == Acceleration{7, 8});
}
