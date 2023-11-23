#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <stdint.h>

struct User {
    uint16_t    start_byte;
    uint8_t     username_size;
    char        username[255];
    uint8_t     pass_size;
    char        pass[255];
    uint16_t    crc_checksum;
};

struct SendMsg {
    uint16_t    start_byte;
    uint8_t     username_size;
    char        username[255];
    uint16_t    msg_size;
    char        msg[500];
    uint16_t    crc_checksum;
};

struct GetMsgs {
    uint16_t    start_byte;
};

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

    // void SendMsg(std::string &username, std::string &msg);
    // void GetAllMsgs();
    
    ~Client();
public:
    int m_clientsock;
private:
    const char *m_port;
    const char *m_servaddr;
    bool m_reg;
    User user{};
};

#endif // Client.hpp
