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
    // Database *m_database;

    // std::unordered_map<int, std::string> user_socket;

    std::map<std::string, int> store;

    Protocol::User user{};
    Protocol::Updatemsg updatemsg{};
    Protocol::Packet packet{};
public:
    Server() {};
    Server(const char *port); //,Database *database);

    void init(const char *port);
    void Start();

    void HandleResponse(int clientsock);

    void Registration(const std::string &usr, int clientsock);
    void Login(const std::string &usr, int clientsock);

    // bool CheckDB(std::string &username, std::string &pass, std::string &msg, int clientsock);

    // void ResetLoginAttempts(int clientsock);

    // std::string GetUsernameFromSocket(int clientsock);
    // void UpdateSocketByUsername(int clientsock, const std::string &username);
    // void RemoveSocket(int clientsock);

    // void ConnectionToDB(Database &database);
    void GetUsersListFromDB(int clientsock);

    void PeerMsgs(int clientsocket);

    void AddMsgsToDB(uint32_t timestamp,std::string &username, std::string &msg);

    void PacketBody(int clientsock, std::string &username, std::string &msg);
    void UpdateMsgs();

    void DisconnectClientFromServer();
    bool DisconnectClient(int clientsock);

    ~Server();
};

#endif // Server.hpp
