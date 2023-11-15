#include "Database.hpp"

Database::Database(const std::string dbname, const std::string user,
            const std::string password, const std::string host,
            const std::string port) 
: m_dbname(dbname), m_user(user), m_password(password), m_host(host), m_port(port) {}

bool Database::ConnectionToServer() {
    try
    {
        m_connection.reset(new pqxx::connection(
            "dbname=" + m_dbname +
            " user=" + m_user +
            " password=" + m_password +
            " host=" + m_host +
            " port=" + m_port
        ));
        // std::string params = "host=localhost port=5432 dbname=mydb user=ines password=pass";

        if (m_connection->is_open()) {
            std::cout << "Connected to the database " << std::endl;
            return true;
        }
        else {
            std::cout << "Connection failed" << std::endl;
            return false;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return false;
    }
}

bool Database::CreateUsersTable() {
    
    try
    {
        // Database connection;
        
        pqxx::work transaction(*m_connection);

        transaction.exec("CREATE TABLE IF NOT EXISTS Users ("
                        "User_id SERIAL PRIMARY KEY, "
                        "Username VARCHAR(255) NOT NULL, "
                        "Password VARCHAR(255) NOT NULL"
                        ")");

        transaction.commit();
        std::cout << "Table 'Users' created successfully " << std::endl;
        return true;
    } catch(const std::exception& e) {
        std::cerr << "Error creating 'Users' table" << e.what() << std::endl;
        return false;
    }
}

bool Database::CreateMsgsTable() {
    try
    {
        pqxx::work transaction(*m_connection);

        transaction.exec("CREATE TABLE IF NOT EXISTS Messages ("
                        "User_id INT REFERENCES Users(User_id) NOT NULL, "
                        "Send_time TIMESTAMP,"
                        "Message VARCHAR(500) NOT NULL"
                        ")");

        transaction.commit();
        std::cout << "Table 'Messages' created successfully " << std::endl;
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error creating 'Messages' table" << e.what() << std::endl;
        return false;
    }
    
}

// void Database::CheckDB() {}

// void Database::AddUser() {}

// void Database::AddMsg() {}

// void Database::PutMsg() {}

Database::~Database() {}


// int main(){
//     Database conn;
//     conn.ConnectionToServer();

//     std::cout << "Helloooo" << std::endl;
//     return 0;
// }



