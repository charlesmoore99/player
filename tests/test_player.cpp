#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "Player.h" // Include the player class

TEST_CASE("Player", "[constructor]") {
    Player p;

    double expected_latitude = 0.0;
    double expected_longitude = 0.0;
    REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.01));
    REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.01));
}


TEST_CASE("Player Movement", "[movement]") {
    Player p;
    p.travel(1.0);

    double expected_latitude = 0.0;
    double expected_longitude = 0.0;
    REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.01));
    REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.01));
}

