// #ifndef CLIENT_HPP
// #define CLIENT_HPP
#pragma once

// #include <iostream>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <netdb.h>
// #include <cstring>
// #include <string>
// #include <unistd.h>
// #include <stdint.h>
// #include "../../Server/headers/Server.hpp"
#include "../../includes.hpp"

struct Response {
    uint8_t OK = 0x01;
    uint8_t ERROR = 0x00;
};

class Client {
public:
    Client();
    Client(const char *port, const char *servaddr);
    bool Start();

    void SendMsgToServer(std::string &msg);

    void UserInfo(size_t &fields);
    bool SendLoginRequest();
    bool SendRegistrationRequest();

    bool isActive() { return m_active; }

    // void PrivateMsgs();
    // void GroupChat();

    // void SendMsg(std::string &username, std::string &msg);
    // void GetAllMsgs();
    
    ~Client();
public:
    int m_clientsock;
    bool m_active;
private:
    const char *m_port;
    const char *m_servaddr;

    Server m_server;
    User user{};
};

// #endif // Client.hpp
