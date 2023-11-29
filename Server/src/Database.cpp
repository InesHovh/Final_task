#include "../headers/Database.hpp"

Database::Database(const std::string dbname, const std::string user,
            const std::string password, const std::string host,
            const std::string port) 
: m_dbname(dbname), m_user(user), m_password(password), m_host(host), m_port(port){
    
    std::cout << "mtel em " << std::endl;
    try
    {

        m_connection = std::make_unique<pqxx::connection>(
            "dbname=" + m_dbname +
            " user=" + m_user +
            " password=" + m_password +
            " host=" + m_host +
            " port=" + m_port );
        

        if (!m_connection)
            throw std::runtime_error("Error: Databse connection");
        if(m_connection != nullptr)
            std::cout << "m_connection is not null " << std::endl;
        if (m_connection->is_open())
            std::cout << "Connection is open " << std::endl;
        else
            std::cout << "Database connection is not open " << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error creating pqxx::connection " << e.what() << std::endl;
    }    
}

bool Database::ConnectionToServer() {
    try
    {
        std::cout << "Want to connect to the server ... " << std::endl;
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

bool Database::CheckUser(const std::string& username) {
    try {
        if (m_connection) {
            pqxx::work transaction(*m_connection);
            
            pqxx::result result = transaction.exec_params(
                "SELECT COUNT(*) FROM Users WHERE username = $1",
                username
            );

            int count = result[0][0].as<int>();

            transaction.commit();

            return count > 0;
        } else {
            std::cerr << "Error: m_connection is null" << std::endl;
            return false;
        }
    } catch(const std::exception& e) {
        std::cerr << "Error checking user in database: " << e.what() << std::endl;
        return false;
    }
}

bool Database::VerifyUser(const std::string& username, const std::string& password) {
    try {
        if (m_connection) {
            pqxx::work transaction(*m_connection);

            pqxx::result result = transaction.exec_params(
                "SELECT COUNT(*) FROM Users WHERE username = $1 AND password = $2",
                username, password
            );

            int count = result[0][0].as<int>();

            transaction.commit();

            return count > 0;
        } else {
            std::cerr << "Error: m_connection is null" << std::endl;
            return false;
        }
    } catch(const std::exception& e) {
        std::cerr << "Error verifying user in database: " << e.what() << std::endl;
        return false;
    }
}



void Database::AddUser(std::string &username, std::string &password) {
    try
    {
        if (m_connection) {
            pqxx::work transaction(*m_connection);
            transaction.exec_params("INSERT INTO Users (username, password) VALUES ($1, $2)",
                                    username, password);
            
            transaction.commit();
        } else {
            std::cerr << "Error: m_connection is null" << std::endl;
            return;
        }
    } catch(const std::exception& e) {
        std::cerr << "Error adding user to database: " << e.what() << std::endl;
        std::cerr << "Exception type: " << typeid(e).name() << std::endl;
    }
    
}

void Database::SetOnlineStatus(const std::string &username, bool status) {
    try
    {
        if(m_connection) {
            pqxx::work transaction(*m_connection);

            transaction.exec_params("UPDATE Users SET online_status = $1 WHERE username = $2",
                status, username);

            if (status) {
                std::cout << "Online Statusi mej em   " << username << "       "  << status << std::endl; 
            }

            transaction.commit();
        } else {
            std::cerr << "Error: m_connection is null " << std::endl;
        }
    } catch(const std::exception& e) {
        std::cerr << "Error " << e.what() << std::endl;
    }
    
}


std::vector<std::string> Database::SendUsersList() {
    try
    {
        if (m_connection) {
            pqxx::work transaction(*m_connection);
            pqxx::result result = transaction.exec("SELECT * FROM Users WHERE online_status = true");

            std::vector<std::string> online_users;

            for (const auto &row : result) {
                online_users.push_back(row["username"].as<std::string>());
            }

            transaction.commit();
            return online_users;
        } else {
            std::cerr << "Error: m_connection is null" << std::endl;
            return {};
        }
    } catch(const std::exception& e) {
        std::cerr << "Error while getting all users list: " << e.what() << std::endl;
        return {};
    }
}

void Database::AddMsg(const std::string &timestamp, std::string &username, std::string &msg) {
    try
    {
        if (m_connection) {
            pqxx::work transaction(*m_connection);
            auto result = transaction.exec_params("SELECT user_id FROM Users WHERE username = $1", 
                                        username);

            if (result.empty()) {
                std::cerr << "Error: User not found for username " << username << std::endl;
                return ;
            }

            int user_id = result[0][0].as<int>();

            transaction.exec_params("INSERT INTO Messages (user_id, send_time, message) VALUES($1, $2, $3)",
                                                user_id, timestamp, msg);

            transaction.commit();
        } else {
            std::cerr << "Error: m_connection is null " << std::endl;
            return ;
        }
    } catch(const std::exception& e) {
        std::cerr << "Error: add msgs to database " << e.what() << std::endl;
    }
}

Database::~Database() {}