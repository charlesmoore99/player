#pragma once

#include <fmt/core.h>
#include <cmath>
#include <mutex>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

/**
 * Player is a representation of an entity with a 3d location 
 * and a 2d velocity vector tangential to the surface of the Earth.
 * 
 */
class Player
{
public:
    std::string name = "Player";

    /// @brief  location
    double lat = 0.0;
    double lon = 0.0;
    double alt = 0.0;

    /// @brief  velocity vector
    double bearing = 0.0;
    double kph = 0.0;

    /**
     * Default Constructor
     * 
     * creates a stationary player at Null Island.
     */
    Player();

    /**
     * Constructor
     *  a player intialized with a location and a velocity vector.
     */
    Player(
        const std::string &playerName,
        const double &playerLat,
        const double &playerLon,
        const double &playerAlt,
        const double &playerBearing,
        const double &playerKph);

    /**
     * returns player as a string of the format:
     *     Player Name: "<name>", Coords:(<lat>, <lon>, <alt>), Velocity: (<bearing>,<speed>)"
     */
    const std::string toString();

    /**
     * returns player as a JSON document
     */
    const std::string toJson();

    /**
     * returns player as a geojson doc
     */
    const std::string toGeoJSON();

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
    void travel(const double hours);

    /**
     * Sets the player's bearing and speed from a JSON document
     * of the format :    {"bearing": 180.0, "kph": 250.0 }
     * 
     * @param velocityDoc a JSON document containing the new bearing/speed
     * 
     */
    void updateVelocity(const std::string &velocityDoc);

    /**
     * Sets the player's bearing and speed from a JSON document
     * of the format :    {"bearing": 180.0, "kph": 250.0 }
     * 
     * @param bearingDegrees the new bearing in degrees
     * @param speedKph the new speed in KPH
     * 
     */
    void updateVelocity(const double bearingDegrees, const double speedKph);

protected:
    std::mutex _playerMutex;

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