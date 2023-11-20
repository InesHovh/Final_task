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

struct Response {
    uint8_t OK;
    uint8_t ERROR;
};

class Client {
public:
    Client();
    Client(const char *port, const char *servaddr);
    bool Start();
    bool SendMsgToServer(const char *msg, size_t size);
    bool ReceiveMsgFromServer(char *buffer, size_t size);
    bool SendLoginRequest();
    bool SendRegistrationRequest();
    bool ReceiveResponse(Response &response);

    std::string check_empty_line(std::string str);

    std::string getUsername() const {
        return user.username;
    }

    void setUsername(std::string username) {
        strncpy(user.username, username.c_str(), sizeof(user.username));
    }

    std::string getPass() const {
        return user.pass;
    }

    void setPass(std::string pass) {
        strncpy(user.pass, pass.c_str(), sizeof(user.pass));
    }

    void AddUser();

    ~Client();
public:
    int m_clientsock;
private:
    const char *m_port;
    const char *m_servaddr;
    User user;
};

#endif // Client.hpp
