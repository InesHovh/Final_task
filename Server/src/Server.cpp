#include "../headers/Server.hpp"

Server::Server(const char *port) : m_servsock(-1) {
    init(port);
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
                }
            }
        }
    }
}

void Server::HandleResponse(int clientsock) {
    // if (!isActive(clientsock)) {
    //     std::cerr << "Client " << clientsock << " is not active. Ignoring this request." << std::endl;
    //     return;
    // }
    
    Request req;
    size_t rec = recv(clientsock, &user, sizeof(user), 0);

    if (rec < 0) {
        std::cerr << "Error receiving data " << clientsock << std::endl;
    } else if(rec == 0) {
        std::cout << "Client " << clientsock << " disconnected" << std::endl;
        close(clientsock);
        // client.m_active = false;
        FD_CLR(clientsock, &m_master);
        m_clients.erase(clientsock);
    } else {
        if (user.start_byte == 0XCBFF){
            if(m_client.empty()){
                Registration(user.username, clientsock);
                send(clientsock, &req.OK, sizeof(req.OK), 0);
                // client.isActive();
            }
            else {
                auto it = m_client.find(user.username);
                if (it != m_client.end() && it->first == user.username && it->second == user.pass) {
                    int snd = send(clientsock, &req.ERROR, sizeof(req.ERROR), 0);
                    std::cout << "This account already exist. Try to register by another username or login." << std::endl;
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
                // std::string str = "You're not registered. Please register before it.";
                // int sndd = send(clientsock, &str.c_str(), sizeof(str.c_str()), 0);
                // if (sndd < 0)
                //     std::cerr << "Failed to send" << std::endl;
                Registration(user.username, clientsock);
                send(clientsock, &req.OK, sizeof(req.OK), 0);
            }
                
            while(i <= 2){
                auto it = m_client.find(user.username);
                if(it != m_client.end() && it->first == user.username && it->second != user.pass) {
                    std::cout << "Incorrect password. Please check it or try to create a new account." << std::endl;
                    ++i;
                if(i == 2)
                    exit(1);
                else {
                    Login(user.username, clientsock);
                    send(clientsock, &req.OK, sizeof(req.OK), 0);
                    }
                }
                int snd = send(clientsock, &req.ERROR, sizeof(req.ERROR), 0);
                if(snd < 0)
                    std::cerr << "Failed to send ERROR response for login" << std::endl;
            }      
            // else {
            //     Login(user.username);
            //     send(m_fdmax, &resp.OK, sizeof(resp.OK), 0);
            // }         
        }
    }
}

void Server::Registration(const std::string &user, int clientsock) {
    std::cout << "Dear " <<   this->user.username << ", you have been successfully registered." << std::endl;
    // isActive(clientsock);
    m_client.insert(std::make_pair(this->user.username, this->user.pass));
}

void Server::Login(const std::string &user, int clientsock) {
    std::cout << "Dear " << this->user.username << ", you have been logged in successfully." << std::endl;
    // isActive(clientsock);
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

// void Server::ConnectionToDB(Database &database) {
//     std::string dbname = "mydb";
//     std::string user = "ines";
//     std::string password = "pass";
//     std::string host = "127.0.0.1";
//     std::string port = "5432";

//     try
//     {
//         if (database.ConnectionToServer()) {
//             std::cout << "Server connected to DB" << std::endl;
//         } else {
//             throw std::runtime_error("Failed to connect to DB");
//         }
//     }
//     catch(const std::exception& e)
//     {
//         std::cerr << "Error " << e.what() << std::endl;
//     }
// }

Server::~Server() {
    close(m_servsock);
}
