#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <iostream>
#include <pqxx/pqxx>

class Database
{
private:
    const std::string m_dbname;
    const std::string m_user;
    const std::string m_password;
    const std::string m_host;
    const std::string m_port;

    std::unique_ptr<pqxx::connection> m_connection;

public:
    Database(const std::string dbname, const std::string user,
            const std::string password, const std::string host,
            const std::string port);

    bool ConnectionToServer();

    // bool CheckUser(const std::string& username);
    bool VerifyUser(const std::string& username, const std::string& password);
    bool CheckUser(const std::string& username);
    void AddClient(std::string &username, std::string &password);

    void AddMsg(const std::string &timestamp,std::string &username, std::string &msg);
    std::vector<std::string> ShowMessages(const std::string &username);
    // void PutMsg();

    ~Database();
};

#endif