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

class Client
{
private:
    std::string m_port;
    int m_clientsock;
    std::string m_servaddr;

    bool m_loggedin;
    
    // std::string m_name;
    // std::string m_nick;
    // std::string m_pass;
    // std::string m_msg;

    // bool m_reg;

    // int m_clientfd;
    // struct sockaddr_in m_clientaddr;
public:
    // Client();
    Client(const char *port, const char * servaddr);
    
    bool Start();
    void Disconnection();

    bool SignUp(const std::string& name, const std::string& nick, const std::string& pass);
    bool SignIn(const std::string& nick, const std::string& pass);

    bool SendMsg(std::string &nick, std::string &msg); 

    // int getPort() const;
    // std::string getName() const;
    // std::string getNick() const;
    // std::string getPass() const;

    // std::string getMsg() const;

    // void setName(std::string &name);
    // void setNick(std::string &nick);
    // void setPass(std::string &pass);

    // void setMsg(std::string &msg);

    // void PrivateChat();

    ~Client();
};

#endif