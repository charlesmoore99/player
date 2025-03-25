#include <fmt/core.h>
#include <cmath>
#include <chrono>
#include <atomic>
#include <csignal>
#include <thread>

#include "Player.h"

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


std::atomic<bool> running(true);

void signalHandler(int signal) {
    if (signal == SIGINT) {
        fmt::print ("\nInterrupt received! Stopping...\n");
        running = false;
    }
}

int main()
{
    std::signal(SIGINT, signalHandler);
    try
    {
        // read vars from env
        std::string playerName = getEnvString("PLAYER_NAME", "Bob");

        // location
        double lat = getEnvDouble("PLAYER_LATITUDE_DEG", 39.7811);
        double lon = getEnvDouble("PLAYER_LONGITUDE_DEG", 84.1104);
        double alt = getEnvDouble("PLAYER_ALTITUDE_DEG", 1251.0);

        // heading & rate of travel
        double bearing = getEnvDouble("PLAYER_BEARING_DEG", 90.0);
        double rate = getEnvDouble("PLAYER_RATE", 150.0);

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

        int updateRate = 1;
        while (running)
        { 
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> deltaTime = currentTime - lastUpdateTime;
            lastUpdateTime = currentTime;

            int sec = std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime).count();
            double hours = sec / (3600.0 * 1e9);
            p.updateDestination(hours);

            fmt::print("{}\n", p.toGeoJSON());
            std::this_thread::sleep_for(std::chrono::seconds(updateRate)); 
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
