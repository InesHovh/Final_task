#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include "../Client/Client.hpp"


class Server
{
private:
    int m_port;
    std::string m_pass;
    int m_servfd;

    std::map<int, Client*> m_client;
    std::map<std::string, int> m_newclient;

    struct sockaddr_in m_servaddr;
public:
    Server();
    Server(int port, std::string &pass);

    void start();

    int getport() const;
    std::string getpass() const;
    
    Client *getclient() const;
    void setclient(Client *client, std::string &nick, int fd);

    void newclient();
    void deleteclient();

    ~Server();
};

#endif