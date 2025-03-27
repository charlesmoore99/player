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

    Player();

    Player(
        const std::string &playerName,
        const double &playerLat,
        const double &playerLon,
        const double &playerAlt,
        const double &playerBearing,
        const double &playerKph);

    std::string toString();

    std::string toGeoJSON();

    void travel(double hours);
    
protected:
    std::tuple<double, double> calculateDestination(
        const double &beginLatDeg,
        const double &beginLonDeg,
        const double &bearingDeg,
        const double &speedKPH,
        const double &timeH);
};