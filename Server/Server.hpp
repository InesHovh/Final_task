#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <vector>
#include "../Database/Database.hpp"

class Database;

class Server {
private:
    int m_servsock;
    fd_set m_master;
    int m_fdmax;

    Database m_database;
public:
    Server() {};
    Server(const char *port);
    void Start();

    void ConnectionToDB(Database &database);
    void DisconnectClient();
    // void SignUp();
    // void SignIn();

    // void AddToDB();
    // void PutFromDB();

    ~Server();
};

#endif // SERVER_HPP
