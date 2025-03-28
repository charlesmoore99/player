#include <fmt/core.h>
#include <cmath>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Player.h"

Player::Player(){}

Player::Player(
    const std::string &playerName,
    const double &playerLat,
    const double &playerLon,
    const double &playerAlt,
    const double &playerBearing,
    const double &playerKph)
    : name(playerName), lat(playerLat), lon(playerLon), alt(playerAlt), bearing(playerBearing), kph(playerKph)
{
}

std::string Player::toString()
{
    return fmt::format("Player Name: \"{}\", Coords:({:.5f}, {:.5f}, {:.1f}), Velocity: ({:.2f}, {:.2f})", name, lat, lon, alt, bearing, kph);
}

std::string Player::toGeoJSON()
{
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

    // Feature type
    document.AddMember("type", "Feature", allocator);

    // Geometry
    rapidjson::Value geometry(rapidjson::kObjectType);
    geometry.AddMember("type", "Point", allocator);

    rapidjson::Value coordinates(rapidjson::kArrayType);
    coordinates.PushBack(lon, allocator);
    coordinates.PushBack(lat, allocator);
    geometry.AddMember("coordinates", coordinates, allocator);

    document.AddMember("geometry", geometry, allocator);

    // Properties
    rapidjson::Value properties(rapidjson::kObjectType);
    rapidjson::Value pName(name.c_str(), allocator);
    properties.AddMember("name", pName, allocator);

    document.AddMember("properties", properties, allocator);

    // Serialize to string
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.SetMaxDecimalPlaces(5);
    document.Accept(writer);

    return buffer.GetString();
}

void Player::travel(double hours)
{
    std::tie(lat, lon) = calculateDestination(lat, lon, bearing, kph, hours);
}

std::tuple<double, double> Player::calculateDestination(
    const double &beginLatDeg,
    const double &beginLonDeg,
    const double &bearingDeg,
    const double &speedKPH,
    const double &timeH)
{
    const double R = 6371.0; // Earth's radius in kilometers

    // Convert degrees to radians
    double beginLat = beginLatDeg * M_PI / 180.0;
    double beginLon = beginLonDeg * M_PI / 180.0;
    double bearing = bearingDeg * M_PI / 180.0;

    // Calculate distance traveled
    double d = speedKPH * timeH;

    // Calculate destination latitude
    double endLat = asin(sin(beginLat) * cos(d / R) + cos(beginLat) * sin(d / R) * cos(bearing));

    // Calculate destination longitude
    double endLon = beginLon + atan2(sin(bearing) * sin(d / R) * cos(beginLat), cos(d / R) - sin(beginLat) * sin(endLat));

    // Convert radians back to degrees
    endLat = endLat * 180.0 / M_PI;
    endLon = endLon * 180.0 / M_PI;

    // normalize the endLon
    if (endLon > 180.0) {
        endLon = fmod(endLon, 180.0);
        endLon = -180.0 + endLon;
    } else if (endLon <= -180.0) {
        endLon = fmod(endLon, 180.0);
        endLon = 180.0 + endLon;
    }

    // normalize the endLon
    if (endLat > 90.0) {
        endLat = fmod(endLat, 90.0);
        endLat = 90.0 - endLat;
    } else if (endLat <= -90.0) {
        endLat = fmod(endLat, 90.0);
        endLat = -90 - endLat;
    }
    return {endLat, endLon};
}