#pragma once

#include <fmt/core.h>
#include <cmath>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class Player
{
public:
    std::string name = "Player";
    double lat = 0.0;
    double lon = 0.0;
    double alt = 0.0;

    double bearing = 0.0;
    double kph = 0.0;

    /**
     * Default Constructor
     */
    Player();

    /**
     * Constructor
     */
    Player(
        const std::string &playerName,
        const double &playerLat,
        const double &playerLon,
        const double &playerAlt,
        const double &playerBearing,
        const double &playerKph);

    /**
     * returns player as a string
     */
    std::string toString();

    /**
     * returns player as a geojons doc
     */
    std::string toGeoJSON();

    /**
     * calculates where the player would be after traveling
     * at its bearing and speed for the provided number of hours,
     * then advances the player to that lat/lon.
     *
     * Calculation assumes the player followed a great circle route
     * to its destination.
     *
     * @param hours.  The fraction number of hours
     */
    void travel(double hours);

protected:
    /// Given a location, a velocity vector, and a time...
    /// calculate how far the player would travel in timeH at speedKPH.
    /// Then calculate where the player would be if it followed
    /// a great circle route to the point that is that distance along
    /// its initial bearing.
    ///
    /// @return a lat/lon of the final coords in a tuple
    std::tuple<double, double> calculateDestination(
        const double &beginLatDeg,
        const double &beginLonDeg,
        const double &bearingDeg,
        const double &speedKPH,
        const double &timeH);
};