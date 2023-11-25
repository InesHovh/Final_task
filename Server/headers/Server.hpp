// #ifndef SERVER_HPP
// #define SERVER_HPP
#pragma once

// #include <iostream>
// #include <cstdlib>
// #include <cstring>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <sys/types.h>
// #include <netdb.h>
// #include <vector>
// #include <array>
// #include <fcntl.h>
// #include <set>
// #include "Database.hpp"
// #include "../../Client/headers/Client.hpp"
#include "../../includes/includes.hpp"

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

    Protocol::User user;
public:
    std::map<std::string, std::string> m_client;
public:
    Server() {};
    Server(const char *port);

    void init(const char *port);
    void Start();

    std::map<std::string, std::string> getClients() const {  return m_client; }
    void setClients(std::map<std::string, std::string> &client) { m_client = client; }

    bool isActive(int clientsock) { return m_clients.find(clientsock) != m_clients.end(); }

    void HandleResponse(int clientsock);

    void Registration(const std::string &usr, int clientsock);
    void Login(const std::string &usr, int clientsock);

    // void ConnectionToDB(Database &database);
    void DisconnectClient();

    ~Server();
};

// #endif // Server.hpp
