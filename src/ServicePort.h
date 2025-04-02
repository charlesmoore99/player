#pragma once

#include <httplib.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <fmt/core.h>

#include "Player.h"

class ServicePort
{

private:
    httplib::Server svr;
    std::string _url;
    int _port;
    std::thread *serverThread = nullptr;
    bool isRunning = false;
    Player &_player;

    inline void SetupServer()
    {
        if (!svr.is_valid())
        {
            printf("server has an error...\n");
            return;
        }

        std::string resp = fmt::format("{{ \"{}\" : \"{}\" }}", "Hello", "World");
        svr.set_error_handler([](const httplib::Request & /*req*/, httplib::Response &res)
        {
            std::string body = "{\"ErrorStatus\" : \"" +  std::to_string(res.status) + "\"}";
            res.set_content(body, "application/json"); 
        });

        svr.Get("/", [ &p =_player ](const httplib::Request &req, httplib::Response &res)
        { 
            res.set_content(p.toJson(), "application/json"); 
            res.status = 200; 
        });
        
        svr.Post("/", [ &p =_player ](const httplib::Request &req, httplib::Response &res)
        {
            try
            {
                p.updateVelocity(req.body);
                res.set_content(p.toJson(), "application/json");
                res.status = 200; 
            }
            catch(const std::invalid_argument& e)
            {
                res.set_content(e.what(), "text/plain");
                res.status = 400; // Bad Request
            }
            

    
        });
    }

public:
    inline ServicePort(const std::string url, const int port, Player &player) : _url(url), _port(port), _player(player)
    {
    }

    inline void StartServer()
    {
        isRunning = true;
        SetupServer();
        serverThread = new std::thread([this]()
        { 
            svr.listen(_url, _port); 
        });

        // Wait for a bit to ensure the server has started
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    inline void StopServer()
    {
        //  std::cerr << "MockServer:Stop"  << std::endl;
        if (isRunning)
        {
            svr.stop();
            if (serverThread && serverThread->joinable())
            {
                serverThread->join();
            }
            delete serverThread;
            serverThread = nullptr;
            isRunning = false;
        }
    }

    inline ~ServicePort()
    {
        StopServer();
    }
};
