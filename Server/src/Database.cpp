#include "../headers/Database.hpp"

Database::Database(const std::string dbname, const std::string user,
            const std::string password, const std::string host,
            const std::string port) 
: m_dbname(dbname), m_user(user), m_password(password), m_host(host), m_port(port){
    
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
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error creating pqxx::connection " << e.what() << std::endl;
    }    
}

bool Database::ConnectionToServer() {
    try
    {
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

void Database::CheckDB() {}

// void Database::AddUser(std::string username, std::string password) {
//     try
//     {
//         // if(!m_connection->is_open())
//         //     throw std::runtime_error("Error: Database conection is not open");
//         // std::cout << "Bareeev" << std::endl;
//         if (m_connection) {
//             pqxx::work transaction(*m_connection);
//             transaction.exec_params("INSERT INTO Users (username, password) VALUES ($1, $2)",
//                                     username, password);
            
//             transaction.commit();
//             std::cout << "User " << username << "registered successfully." << std::endl;
//         } else {
//             std::cerr << "Error: m_connection is null" << std::endl;
//         }
//     } catch(const std::exception& e) {
//         std::cerr << "Error adding user to database: " << e.what() << std::endl;
//         std::cerr << "Exception type: " << typeid(e).name() << std::endl;
//     }
    
// }

Database::~Database() {}