#include <fmt/core.h>
#include <cmath>
#include <chrono>
#include <thread>

std::string getEnvString(std::string name, std::string defaultVal)
{
    const char *valPtr = std::getenv(name.c_str());
    if (!valPtr)
    {
        return defaultVal;
    }
    std::string val = valPtr;
    return val;
}

double getEnvDouble(std::string name, double defaultVal)
{
    const char *valPtr = std::getenv(name.c_str());
    if (!valPtr)
    {
        return defaultVal;
    }
    double val = std::stod(valPtr);
    ;
    return val;
}

struct Coordinates
{
    double latitude;
    double longitude;
};

std::tuple<double, double> calculateDestination(
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
    double endlon = beginLon + atan2(sin(bearing) * sin(d / R) * cos(beginLat), cos(d / R) - sin(beginLat) * sin(endLat));

    // Convert radians back to degrees
    endLat = endLat * 180.0 / M_PI;
    endlon = endlon * 180.0 / M_PI;

    return {endLat, endlon};
}

class Player
{
public:
    std::string name;
    double lat;
    double lon;
    double alt;

    double bearing;
    double kph;

    inline Player(
        const std::string &playerName,
        const double &playerLat,
        const double &playerLon,
        const double &playerAlt,
        const double &playerBearing,
        const double &playerKph)
        : name(playerName), lat(playerLat), lon(playerLon), alt(playerAlt), bearing(playerBearing), kph(playerKph)
    {
    }

    inline std::string toString()
    {
        return fmt::format("Player Name: \"{}\", Coords:({}, {}, {}), Velocity: ({}, {})", name, lat, lon, alt, bearing, kph);
    }

    inline void updateDestination(double hours)
    {
        std::tie(lat, lon) = calculateDestination(lat, lon, bearing, kph, hours);
    }
};

int main()
{

    try
    {

        // read vars from env
        std::string playerName = getEnvString("PLAYER_NAME", "Bob");

        // location
        double lat = getEnvDouble("PLAYER_LATITUDE_DEG", 10.0);
        double lon = getEnvDouble("PLAYER_LONGITUDE_DEG", 20.0);
        double alt = getEnvDouble("PLAYER_ALTITUDE_DEG", 30.0);

        // heading & rate of travel
        double bearing = getEnvDouble("PLAYER_BEARING_DEG", 40.0);
        double rate = getEnvDouble("PLAYER_RATE", 50.0);

        if (lat < -90.0 || lat > 90.0)
        {
            throw std::out_of_range(fmt::format("Latitude value ({}) is out of range. It must be in the range (-90.0 , 90.0)", lat));
        }

        if (lon < -180.0 || lon > 180.0)
        {
            throw std::out_of_range(fmt::format("Longitude value ({}) is out of range. It must be in the range [-180.0 , 180.0)", lon));
        }

        if (bearing < 0.0 || bearing >= 360.0)
        {
            throw std::out_of_range(fmt::format("Bearing value ({}) is out of range. It must be in the range (0.0->360.0]", bearing));
        }

        if (rate < 0.0)
        {
            throw std::out_of_range(fmt::format("Rate value ({}) is out of range.  It must be greather than or equal to 0.", bearing));
        }

        Player p(playerName, lat, lon, alt, bearing, rate);
        fmt::print("{}\n", p.toString());

        // event loop to update the player location
        auto lastUpdateTime = std::chrono::high_resolution_clock::now();

        double updateRate = 1;
        for (unsigned int x = 0; x < 100; x++)
        { // Game loop (replace with your exit condition)
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> deltaTime = currentTime - lastUpdateTime;
            lastUpdateTime = currentTime;

            int sec = std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime).count();
            double hours = sec / (3600.0 * 1e9);
            fmt::print("{}\n", hours);
            p.updateDestination(hours);

            fmt::print("{}\n", p.toString());

            std::this_thread::sleep_for(std::chrono::duration<double>(1.0 / updateRate)); // Control update rate
        }
    }
    catch (const std::out_of_range &e)
    {
        fmt::print("Data Validation exception caught: {} !\n", e.what());
        return 1;
    }
    catch (const std::exception &e)
    {
        fmt::print("Exception caught: {} !\n", e.what());
        return 1;
    }
    catch (...)
    {
        fmt::print("Unknown Exception caught!\n");
        return 1;
    }
    return 0;
}
