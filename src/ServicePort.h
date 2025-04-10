#pragma once

#include <httplib.h>
#include <string>
#include <iostream>
#include <thread>
#include <cctype>
#include <algorithm>
#include <fmt/core.h>

#include "Player.h"

class ServicePort
{

private:
    std::string _url;
    int _port;
    Player& _player;

    httplib::Server svr;
    std::unique_ptr<std::thread> serverThread = nullptr;


    inline void SetupServer()
    {
        if (!svr.is_valid())
        {
            printf("server has an error...\n");
            return;
        }

        svr.set_error_handler([](const httplib::Request& /*req*/, httplib::Response& res)
        {
            res.set_content(fmt::format("{\"ErrorStatus\" : \"{}\"}", std::to_string(res.status)), "application/json"); });

        // GET returns the Player as a JSON object
        svr.Get("/", [&p = _player](const httplib::Request& req, httplib::Response& res)
        {
            std::string ct = req.get_header_value("Content-Type");
            std::transform(ct.begin(), ct.end(), ct.begin(), ::tolower);
            if (ct == "application/geo+json")
            {
                res.set_content(p.toGeoJSON(), "application/geo+json");
            }
            else
            {
                res.set_content(p.toJson(), "application/json");
            }
            res.status = 200; });

        // POST updates the Players velocity vector and Player as a JSON object
        svr.Post("/", [&p = _player](const httplib::Request& req, httplib::Response& res)
        {
            try
            {
                p.updateVelocity(req.body);
                std::string ct = req.get_header_value("Content-Type");
                std::transform(ct.begin(), ct.end(), ct.begin(), ::tolower);
                if (ct == "application/geo+json")
                {
                    res.set_content(p.toGeoJSON(), "application/geo-json");
                }
                else
                {
                    res.set_content(p.toJson(), "application/json");
                }
                res.status = 200;
            }
            catch (const std::invalid_argument& e)
            {
                res.set_content(e.what(), "text/plain");
                res.status = 400; // Bad Request
            } });
    }

public:
    /**
     * @brief Given a player.  Creates an http server that allows interaction
     * with a Player.
     *
     * A GET request sent to the URL/Port returns a JSON representation of
     * the Player's current state
     * A POST request with an JSON body updates the Player's velocity vector.
     *
     * @param url the url of the network interface that will accept
     * connections.  Values include:
     * - 0.0.0.0 - to accept requests from anywhere
     * - localhost - to accept requests only originating from localhost
     * (NOTE: if the server is running inside a docker container is will
     * only accept requests from inside the container. Requests originating
     * from the host or any other container will NTO be accepted.)
     * - the IP address or DNS name associated with one of the network
     * interfaces on the container/host this server it running on
     * @param port the port that will be listend to
     * @param player a reference to the Player that will served
     */
    inline ServicePort(const std::string url, const int port, Player& player)
        : _url(url), _port(port), _player(player)
    {
    }

    /**
     * Starts the server in a thread
     */
    inline void StartServer()
    {
        SetupServer();
        serverThread = std::make_unique<std::thread>([this]()
        {
            svr.listen(_url, _port);
        });

        // Wait for a bit to ensure the server has started
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    inline void StopServer()
    {
        if (serverThread)
        {
            svr.stop();
            if (serverThread && serverThread->joinable())
            {
                serverThread->join();
            }
            serverThread.reset();
        }
    }

    /**
     * Destructor
     */
    inline ~ServicePort()
    {
        StopServer();
    }
};
