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
#include <unistd.h>
#include <vector>

class Client {
public:
    Client(const char *port, const char *servaddr);
    bool Start();
    bool SendMsgToServer(const std::string &msg);
    bool ReceiveMsgFromServer(std::string &receivedMsg);
    ~Client();

    bool SignUp();
    bool SignIn();
// protected:
    int m_clientsock;
private:
    const char *m_port;
    const char *m_servaddr;
    std::string m_login;
    std::string m_pass;
};

#endif