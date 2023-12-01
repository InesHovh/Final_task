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

void Database::AddClient(std::string &username, std::string &password) {
    try
    {
        if (m_connection) {
            pqxx::work transaction(*m_connection);
            transaction.exec_params("INSERT INTO Client (name, password) VALUES ($1, $2)",
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

bool Database::CheckUser(const std::string& username) {
    try {
        if (m_connection) {
            pqxx::work transaction(*m_connection);
            
            pqxx::result result = transaction.exec_params(
                "SELECT COUNT(*) FROM Client WHERE name = $1",
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
                "SELECT COUNT(*) FROM Client WHERE name = $1 AND password = $2",
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

void Database::AddMsg(const std::string &timestamp, std::string &username, std::string &msg) {
    try
    {
        if (m_connection) {
            pqxx::work transaction(*m_connection);
            auto result = transaction.exec_params("SELECT client_id FROM Client WHERE name = $1", 
                                        username);

            if (result.empty()) {
                std::cerr << "Error: User not found for username " << username << std::endl;
                return ;
            }

            int client_id = result[0][0].as<int>();

            transaction.exec_params("INSERT INTO Messages (client_id, send_time, message) VALUES($1, $2, $3)",
                                                client_id, timestamp, msg);

            transaction.commit();
        } else {
            std::cerr << "Error: m_connection is null " << std::endl;
            return ;
        }
    } catch(const std::exception& e) {
        std::cerr << "Error: add msgs to database " << e.what() << std::endl;
    }
}

std::vector<std::string> Database::ShowMessages(const std::string &username) {
    std::vector<std::string> messages;

    try {
        if (m_connection) {
            pqxx::work transaction(*m_connection);
            
            pqxx::result result = transaction.exec_params(
                    "SELECT send_time, message FROM Messages WHERE client_id = (SELECT client_id FROM Client WHERE name = $1 LIMIT 1)", username
                );


            if (!result.empty()) {
                for(const auto &row : result) {
                    std::string sendTime = row[0].as<std::string>();
                    std::string message = row[1].as<std::string>();

                    std::string fullMessage = sendTime + ":" + message;
                    messages.push_back(fullMessage);
                }
            } else {
                std::cerr << "No messages found for the given username." << std::endl;
            }

            transaction.commit();
        } else {
            std::cerr << "Error: m_connection is null " << std::endl;
        }
    }
    catch(const std::exception& e) {
        std::cerr << "Error: while getting and sending messages " << e.what() << std::endl;
    }

    return messages;
}

Database::~Database() {}