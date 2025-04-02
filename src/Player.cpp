#include <fmt/core.h>
#include <cmath>
#include <stdexcept>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

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
    std::lock_guard<std::mutex> lock(_playerMutex);
    return fmt::format("Player Name: \"{}\", Coords:({:.5f}, {:.5f}, {:.1f}), Velocity: ({:.2f}, {:.2f})", name, lat, lon, alt, bearing, kph);
}

std::string Player::toJson()
{
    std::lock_guard<std::mutex> lock(_playerMutex);

    rapidjson::Document document;
    document.SetObject(); // Set the document as a JSON object (key-value pairs)
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("name", rapidjson::Value(name.c_str(), name.length()), allocator);
    document.AddMember("lat", lat, allocator);
    document.AddMember("lon", lon, allocator);
    document.AddMember("alt", alt, allocator);
    document.AddMember("bearing", bearing, allocator);
    document.AddMember("kph", kph, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
}

std::string Player::toGeoJSON()
{
    std::lock_guard<std::mutex> lock(_playerMutex);

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
    std::lock_guard<std::mutex> lock(_playerMutex);
    std::tie(lat, lon) = calculateDestination(lat, lon, bearing, kph, hours);
}

void Player::updateVelocity(const std::string& velocityDoc)
{
    std::lock_guard<std::mutex> lock(_playerMutex);

    rapidjson::Document document;
    document.Parse(velocityDoc.c_str());

    if (document.HasParseError()) {
        std::string error = fmt::format( "JSON Parse Error: {} at offset {} ",  rapidjson::GetParseError_En(document.GetParseError()), document.GetErrorOffset());
        fmt::println("{}", error);
        throw std::invalid_argument(error);
    }
    
    if (document.IsObject() 
        && document.HasMember("kph") 
        && document["kph"].IsDouble()
        && document.HasMember("bearing") 
        && document["bearing"].IsDouble()) 
    {
        bearing = document["bearing"].GetDouble();
        kph = document["kph"].GetDouble();
    }
    else 
    {
        std::string error = fmt::format( "Invalid data types in velocity document");
        fmt::println("{}", error);
        throw std::invalid_argument(error);
    }
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