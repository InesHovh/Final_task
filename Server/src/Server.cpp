#include "../headers/Server.hpp"

Server::Server(const char *port, Database *database) : m_servsock(-1) {
    m_database = database;
    init(port);
    ConnectionToDB(*m_database);
    Start();
}

void Server::init(const char *port) {
    struct addrinfo servaddr, *result;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.ai_family = AF_UNSPEC;
    servaddr.ai_socktype = SOCK_STREAM;
    servaddr.ai_flags = AI_PASSIVE;

    if (getaddrinfo(nullptr, port, &servaddr, &result) < 0) {
        std::cerr << "getaddrinfo error" << std::endl;
        exit(EXIT_FAILURE);
    }

    m_servsock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_servsock < 0) {
        std::cerr << "Failed to create the socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    fcntl(m_servsock, F_SETFL, O_NONBLOCK);

    if (bind(m_servsock, result->ai_addr, result->ai_addrlen) < 0) {
        std::cerr << "Failed to bind" << std::endl;
        close(m_servsock);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    FD_ZERO(&m_master);
    FD_SET(m_servsock, &m_master);
    m_fdmax = m_servsock;
}

void Server::Start() {
    if (listen(m_servsock, 10) < 0) {
        std::cerr << "Failed to listen" << std::endl;
        close(m_servsock);
        exit(EXIT_FAILURE);
    }
    std::cout << "Server started. Waiting for connections..." << std::endl;

    timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 300000;

    while (true) {
        fd_set rd = m_master;
        if (select(m_fdmax + 1, &rd, nullptr, nullptr, nullptr) < 0) {
            std::cerr << "select error" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i <= m_fdmax; ++i) {
            if (FD_ISSET(i, &rd)) {
                if (i == m_servsock) {
                    int clientsock = accept(m_servsock, nullptr, nullptr);
                    if (clientsock < 0) {
                        std::cerr << "Failed to accept connection" << std::endl;
                        continue;
                    }
                    FD_SET(clientsock, &m_master);
                    if (clientsock > m_fdmax) {
                        m_fdmax = clientsock;
                    }
                    std::cout << "Connected " << std::endl;
                } else {
                    HandleResponse(i);
                    std::cout << "Barev dzez  " << std::endl;
                    GetUsersListFromDB(i);
                }
            }
        }
    }
}

void Server::HandleResponse(int clientsock) {
    Request req;
    size_t rec = recv(clientsock, &user, sizeof(user), 0);

    if (rec < 0) {
        std::cerr << "Error receiving data " << clientsock << std::endl;
    } else if(rec == 0) {
        std::cout << "Client " << clientsock << " disconnected" << std::endl;
        close(clientsock);
        FD_CLR(clientsock, &m_master);
        m_clients.erase(clientsock);
    } else {
        if (user.start_byte == 0XCBFF){
            if(m_client.empty()){
                Registration(user.username, clientsock);
                send(clientsock, &req.OK, sizeof(req.OK), 0);
            }
            else {
                // auto it = m_client.find(user.username);
                // if (it != m_client.end() && it->first == user.username && it->second == user.pass)
                std::string msg = "REGISTER";
                std::string username(this->user.username);
                std::string pass(this->user.pass);
                if (CheckDB(username, pass, msg, clientsock)){
                    int snd = send(clientsock, &req.ERROR, sizeof(req.ERROR), 0);
                    // std::cout << "This account already exist. Try to register by another username or login." << std::endl;
                    if(snd < 0)
                        std::cout << "Failed to send ERROR reqonse for registration";
                }
                else {
                    Registration(user.username, clientsock);
                    send(clientsock, &req.OK, sizeof(req.OK), 0);
                }
            }
        }
        else if(user.start_byte == 0xCBAE) {
            int i = 0;
            
            if(m_client.empty()) {
                Registration(user.username, clientsock);
                send(clientsock, &req.OK, sizeof(req.OK), 0);
            }
                
            while(i <= 2){
                // auto it = m_client.find(user.username);
                // if(it != m_client.end() && it->first == user.username && it->second != user.pass)
                std::string msg = "LOGIN";
                std::string username(this->user.username);
                std::string pass(this->user.pass);
                if (CheckDB(username, pass, msg, clientsock)) {
                    std::cout << "Incorrect password. Please check it or try to create a new account." << std::endl;
                    ++i;
                    if (i == 2) {
                        std::cerr << "Maximum login attempts reached. Exiting loop..." << std::endl;
                        break;
                    } else {
                        Login(user.username, clientsock);
                        send(clientsock, &req.OK, sizeof(req.OK), 0);
                    }
                }
                int snd = send(clientsock, &req.ERROR, sizeof(req.ERROR), 0);
                if(snd < 0)
                    std::cerr << "Failed to send ERROR response for login" << std::endl;
            }       
        }
    } 
    // if(user.start_byte == 0xCAFE) 
    //     GetUsersListFromDB(clientsock);
}

void Server::Registration(const std::string &user, int clientsock) {
    std::cout << "Dear " <<   this->user.username << ", you have been successfully registered." << std::endl;
    // m_client.insert(std::make_pair(this->user.username, this->user.pass));
    std::string username(this->user.username);
    std::string pass(this->user.pass);
    // AddToDB(username, pass);
    m_database->AddUser(username, pass);
}

void Server::Login(const std::string &user, int clientsock) {
    std::cout << "Dear " << this->user.username << ", you have been logged in successfully." << std::endl;
}

void Server::DisconnectClient() {
    std::cout << "Server is disconnecting clients..." << std::endl;

    for (int i = 0; i <= m_fdmax; ++i) {
        if (FD_ISSET(i, &m_master) && i != m_servsock) {
            std::string disconnectMsg = "SERVER_DISCONNECT";
            if (send(i, disconnectMsg.c_str(), disconnectMsg.size(), 0) < 0) {
                std::cerr << "Failed to send disconnect message to client" << std::endl;
            }
            close(i);
            FD_CLR(i, &m_master);
        }
    }
}

void Server::ConnectionToDB(Database &database) {
    // std::string dbname = "mydb";
    // std::string user = "ines";
    // std::string password = "pass";
    // std::string host = "127.0.0.1";
    // std::string port = "5432";

    try
    {
        if (database.ConnectionToServer()) {
            std::cout << "Server connected to DB" << std::endl;
        } else {
            throw std::runtime_error("Failed to connect to DB");
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error " << e.what() << std::endl;
    }
}

bool Server::CheckDB(std::string &username, std::string &pass, std::string &msg, int clientsock) {
    if (msg == "REGISTER") {
        if (m_database->CheckUser(username)) {
            std::cout << "This account already exists. Try to register with another username or login." << std::endl;
            return true;
        } else {
            Registration(username, clientsock);
            return false;
        }
    } else if (msg == "LOGIN") {
        if (m_database->VerifyUser(username, pass)) {
            std::cout << "User login successful." << std::endl;
            Login(username, clientsock);
            return true;
        } else {
            std::cout << "Incorrect username or password. Please check and try again." << std::endl;
            return false;
        }
    } else {
        std::cerr << "Invalid message type for database check" << std::endl;
        return false;
    }
}

// void Server::GetUsersListFromDB(int clientsock) {
//     uint16_t resp;
//     size_t rec = recv(clientsock, &resp, sizeof(resp), 0);

//     if (rec < 0) {
//         std::cerr << "Error while receiving message " << std::endl;
//         return ;
//     }

//     if (resp == 0xCAFE) {
//         std::vector<std::string> users = m_database->SendUsersList();

//         for(const auto &username : users) {
//             send(clientsock, username.c_str(), username.size(), 0);
//         }
//     }
// }


void Server::GetUsersListFromDB(int clientsock) {
    uint16_t start_byte = 0xCAFE;


    uint16_t resp;
    size_t rec = recv(clientsock, &resp, sizeof(resp), 0);

    if (rec < 0) {
        std::cerr << "Error receiving response from client" << std::endl;
        return;
    }

    if (resp == 0xCAFE) {
        std::vector<std::string> users = m_database->SendUsersList();

        std::cout << "mtnum a CAFE" << std::endl;

        uint16_t numUsers = static_cast<uint16_t>(users.size());
        send(clientsock, &numUsers, sizeof(numUsers), 0);

        for (const auto& username : users) {
            size_t len = username.size();
            std::cout << username << std::endl;
            send(clientsock, &len, sizeof(len), 0);
            send(clientsock, username.c_str(), len, 0);
        }
    }
}



Server::~Server() {
    close(m_servsock);
}
