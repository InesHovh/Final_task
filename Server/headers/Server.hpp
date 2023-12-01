#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>
#include <thread>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <ctime>
#include <string>
#include <sstream>
#include <fcntl.h>
// #include "../../Utils/util.hpp"
#include "Database.hpp"

class Server
{
public:
    Server(const char *port, Database * database);
    ~Server();
    void init(const char *port);
    void start();

private:
    static const int MAX_SOCKETS = 10;
    Database *database;
    int m_servsock;

    struct AcceptedSocket
    {
        int acceptedSocketFD;
        struct sockaddr_in address;
        int error;
        bool acceptedSuccessfully;
    };

    struct Packet {
        std::string timestamp;
        std::string name;
        std::string msg;
    };

    Packet packet{};

    AcceptedSocket acceptedSockets[MAX_SOCKETS];
    int acceptedSocketsCount;

    int serverSocketFD;
public:
    static void *receiveAndPrintIncomingData(void *data);
    void receiveIncomingData(int socketFD);
    void SenddMessageToTheClients(char *buffer, int senderSocketFD);
    AcceptedSocket *acceptIncomingConnection();

    void ConnectionToDB(Database &database);
    void PacketBody(const std::string &name, const std::string &msg);
    void startAccepting();
};

#endif // SERVER_HPP
