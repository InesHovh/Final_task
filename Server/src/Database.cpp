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

void Database::AddClient(std::string &username) {
    try
    {
        if (m_connection) {
            pqxx::work transaction(*m_connection);
            transaction.exec_params("INSERT INTO Client (name) VALUES ($1)",
                                    username);
            
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

Database::~Database() {}