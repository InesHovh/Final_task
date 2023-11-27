#ifndef SERVER_HPP
#define SERVER_HPP

#include "../../includes/includes.hpp"
#include "Database.hpp"

struct Request {
    uint8_t OK = 0x01;
    uint8_t ERROR = 0x00;
};

class Server {
private:
    int m_servsock;
    fd_set m_master;
    int m_fdmax;
    std::set<int> m_clients;
    Database *m_database;


    Protocol::User user{};
    Protocol::Updatemsg updatemsg{};
    Protocol::Packet packet{};
public:
    Server() {};
    Server(const char *port, Database *database);

    void init(const char *port);
    void Start();

    void HandleResponse(int clientsock);

    void Registration(const std::string &usr, int clientsock);
    void Login(const std::string &usr, int clientsock);

    bool CheckDB(std::string &username, std::string &pass, std::string &msg, int clientsock);


    void ConnectionToDB(Database &database);
    void GetUsersListFromDB(int clientsock);

    void AddMsgsToDB(std::string &username, std::string &msg, int clientsock);

    void PacketBody(int clientsock);
    void UpdateMsgs();

    void DisconnectClient();

    ~Server();
};

#endif // Server.hpp
