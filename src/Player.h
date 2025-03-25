#pragma once

#include <fmt/core.h>
#include <cmath>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


class Player
{
public:
    std::string name;
    double lat;
    double lon;
    double alt;

    double bearing;
    double kph;

    Player(
        const std::string &playerName,
        const double &playerLat,
        const double &playerLon,
        const double &playerAlt,
        const double &playerBearing,
        const double &playerKph);

    std::string toString();

    std::string toGeoJSON();

    void updateDestination(double hours);
    
protected:
    std::tuple<double, double> calculateDestination(
        const double &beginLatDeg,
        const double &beginLonDeg,
        const double &bearingDeg,
        const double &speedKPH,
        const double &timeH);
};