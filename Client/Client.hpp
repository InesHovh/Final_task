#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Client
{
private:
    int m_port;
    
    std::string m_name;
    std::string m_nick;
    std::string m_pass;

    int m_clientfd;
    struct sockaddr_in m_clientaddr;
public:
    Client();
    Client(int port, std::string &name, std::string &nick,
            std::string &pass);
    
    void run();

    int getport() const;
    std::string getname() const;
    std::string getnick() const;
    std::string getpass() const;

    void setname(std::string &name);
    void setnick(std::string &nick);
    void setpass(std::string &pass);

    ~Client();
};

#endif