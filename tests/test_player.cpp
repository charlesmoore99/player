#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "Player.h" // Include the player class

TEST_CASE("Player", "[constructor]")
{

    SECTION("Default")
    {
        Player p;
        double expected_latitude = 0.0;
        double expected_longitude = 0.0;
        REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.01));
        REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.01));
    }
    SECTION("WithParams")
    {
        Player p("Fruit", 1.0, 2.0, 3.0, 4.0, 100);

        std::string expected_name("Fruit");
        double expected_latitude = 1.0;
        double expected_longitude = 2.0;
        double expected_altitude = 3.0;
        double expected_bearing = 4.0;
        double expected_kph = 100.0;
        REQUIRE(p.name == expected_name);
        REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.01));
        REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.01));
        REQUIRE(p.alt == Catch::Approx(expected_altitude).margin(0.01));
        REQUIRE(p.bearing == Catch::Approx(expected_bearing).margin(0.01));
        REQUIRE(p.kph == Catch::Approx(expected_kph).margin(0.01));
    }
}

TEST_CASE("Player Movement", "[movement]")
{
    SECTION("NotMoving")
    {
        Player p;
        p.travel(1.0);
        double expected_latitude = 0.0;
        double expected_longitude = 0.0;
        REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.01));
        REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.01));
    }

    SECTION("Moving")
    {
        Player p;
        p.bearing = 135.0;
        p.kph = 100.0;
        p.travel(1.0);
        double expected_latitude = -0.63590;
        double expected_longitude = 0.63590;
        REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.0001));
        REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.0001));
    }

    SECTION("CrossingAntiMeridianEast")
    {
        Player p;
        p.lat = 0.1;
        p.lon = 179.9;
        p.bearing = 135.0;
        p.kph = 100.0;
        p.travel(1.0);
        double expected_latitude = -0.53590;
        double expected_longitude = -179.46405;
        REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.0001));
        REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.0001));
    }

    SECTION("CrossingAntiMeridianWest")
    {
        Player p;
        p.lat = 0.1;
        p.lon = -179.9;
        p.bearing = 225.0;
        p.kph = 100.0;
        p.travel(1.0);
        double expected_latitude = -0.53590;
        double expected_longitude = 179.46405;
        REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.0001));
        REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.0001));
    }

    SECTION("CrossingPoleNorth")
    {
        Player p;
        p.lat = 89.9;
        p.lon = 45.0;
        p.bearing = 0.0;
        p.kph = 100.0;
        p.travel(1.0);
        double expected_latitude = 89.20067;
        double expected_longitude = -135.0;
        REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.0001));
        REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.0001));
    }

    SECTION("CrossingPoleSouth")
    {
        Player p;
        p.lat = -89.9;
        p.lon = 0.0;
        p.bearing = 180.0;
        p.kph = 100.0;
        p.travel(1.0);
        double expected_latitude = -89.20067;
        double expected_longitude = 180.0;
        REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.0001));
        REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.0001));
    }

    SECTION("EndsAtNorthPole")
    {
        Player p;
        p.lat = 89.90991;
        p.lon = 0.0;
        p.bearing = 0.0;
        p.kph = 10.0;
        p.travel(1.0);
        double expected_latitude = 90.0;
        double expected_longitude = 0.0;
        REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.0001));
        REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.0001));
    }

    SECTION("EndsAtSouthPole")
    {
        Player p;
        p.lat = -89.90991;
        p.lon = 0.0;
        p.bearing = 180.0;
        p.kph = 10.0;
        p.travel(1.0);
        double expected_latitude = -90.0;
        double expected_longitude = 0.0;
        REQUIRE(p.lat == Catch::Approx(expected_latitude).margin(0.0001));
        REQUIRE(p.lon == Catch::Approx(expected_longitude).margin(0.0001));
    }
}
