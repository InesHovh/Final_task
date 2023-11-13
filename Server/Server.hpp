#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <map>
#include <unistd.h>
#include <cstring>
#include "../Client/Client.hpp"


class Server
{
private:
    std::string m_port;
    int m_servsock;
    // std::string m_pass;

    // std::map<int, Client*> m_client;
    // std::map<std::string, int> m_newclient;

    // struct sockaddr_in m_servaddr;

    // struct addrinfo m_sockaddr;
    // struct addrinfo *result;
public:
    // Server();
    Server(const char *port);

    void Start();
    void setClient(int clientsock);
    // void Stop();

    // int getPort() const;
    // std::string getPass() const;
    
    // Client *getClient() const;

    void NewClient();
    void DeleteClient();

    void Registration();
    void AlreadyExist();

    // void AddToDB();
    // void PutFromDB();

    ~Server();
};

#endif