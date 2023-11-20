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
#include <array>
#include <fcntl.h>
// #include <sys/socket.h>
#include "Database.hpp"

struct Updatemsg {
    uint16_t    start_byte;
    uint16_t    packet_counts;
    uint16_t    crc_checksum;
};

struct Packet {
    uint16_t    start_byte;
    uint32_t    timestamp;
    uint8_t     username_size;
    char        username[255];
    uint16_t    msg_size;
    char        msg[500];
    uint16_t    crc_checksum;
};

struct Request {
    uint8_t OK = 0x01;
    uint8_t ERROR = 0x00;
};

class Server {
private:
    int m_servsock;
    fd_set m_master;
    int m_fdmax;

    Database m_database;
public:
    Server() {};
    Server(const char *port);

    void init(const char *port);
    void Start();

    void ConnectionToDB(Database &database);
    void DisconnectClient();

    void LoginResponse(int clientsock, uint8_t response);
    void LoginRequest();

    void RegResponse(int clientsock, uint8_t response);
    void RegRequest();

    // void SignUp();
    // void SignIn();

    // void AddToDB();
    // void PutFromDB();

    ~Server();
};

#endif // SERVER_HPP
