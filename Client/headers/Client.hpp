#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include "../../includes/includes.hpp"

struct Response {
    uint8_t OK = 0x01;
    uint8_t ERROR = 0x00;
};

class Client {
public:
    Client();
    Client(const char *port, const char *servaddr);
    bool Start();

    bool SendMsgToServer(std::string &msg);

    void UserInfo(size_t &fields);
    void SendLoginRequest();
    void SendRegistrationRequest();

    void GetUsersList();

    void PrivateMsgs(const std::string &user, const std::string &msg);

    void SendMsg(std::string &username, std::string &msg);
    
    void GetAllMsgs();
    
    ~Client();
public:
    int m_clientsock;
    bool m_active;
    Protocol::User user{};
private:
    const char *m_port;
    const char *m_servaddr;

};

#endif // Client.hpp
