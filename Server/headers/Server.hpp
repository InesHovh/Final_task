#ifndef SERVER_HPP
#define SERVER_HPP
// #pragma once

#include <iostream>
#include <set>
#include <map>
#include "../../includes/includes.hpp"
// #include "/Users/ihovhann/Desktop/Final_task/includes/includes.hpp"

// using namespace Protocol;

struct Request {
    uint8_t OK = 0x01;
    uint8_t ERROR = 0x00;
};

class Server {
private:
    int m_servsock;
    fd_set m_master;
    int m_fdmax;
    std::set<int> m_clients;

public:
    Protocol::User user;
    std::map<std::string, std::string> m_client;
public:
    Server() {};
    Server(const char *port);

    void init(const char *port);
    void Start();

    void HandleResponse(int clientsock);

    void Registration(const std::string &usr, int clientsock);
    void Login(const std::string &usr, int clientsock);

    // void ConnectionToDB(Database &database);
    void DisconnectClient();

    ~Server();
};

#endif // Server.hpp
