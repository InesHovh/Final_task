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
    Database() {};
    Database(const std::string dbname, const std::string user,
            const std::string password, const std::string host,
            const std::string port);

    bool ConnectionToServer();

    bool CheckUser(const std::string& username);
    bool VerifyUser(const std::string& username, const std::string& password);

    void AddUser(std::string &username, std::string &password);

    void SetOnlineStatus(const std::string &username, bool status);

    std::vector<std::string> SendUsersList();

    void AddMsg(uint32_t timestamp,std::string &username, std::string &msg);
    // void PutMsg();

    ~Database();
};

#endif