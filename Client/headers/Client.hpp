#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <stdint.h>
#include <vector>
#include <array>
#include <map>

struct User {
    uint16_t    start_byte;
    uint8_t     username_size;
    char        username[255];
    uint8_t     pass_size;
    char        pass[255];
    uint16_t    crc_checksum;
};

struct Response {
    uint8_t OK ;
    uint8_t ERROR ;
};

class Client {
public:
    Client();
    Client(const char *port, const char *servaddr);
    bool Start();
    bool SendMsgToServer(const std::string &msg);
    bool ReceiveMsgFromServer(std::string &receivedMsg);

    std::string check_empty_line(std::string str);

    std::string getUsername() const {
        User user;
        return user.username;
    }
    void setUsername(std::string username) {
        this->getUsername() = username;
    }

    std::string getPass() const {
        User user;

        return user.pass;
    }
    void setPass(std::string pass) {
        this->getPass() = pass;
    }

    std::string LoginRequest(const std::string &request);
    uint8_t  LoginResponse();

    std::string RegRequest(const std::string &request);
    uint8_t  RegResponse();

    void AddUser();


    void SignUp();
    void SignIn();

    // void SendMessage();
    // void GetAllMessages();


    ~Client();
public:
    int m_clientsock;
private:
    std::map<std::string, std::string> clients;

    const char *m_port;
    const char *m_servaddr;
};

#endif