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
                    
                    std::cout << "Connected " << clientsock  << "  client" << std::endl;
                } else {
                    HandleResponse(i);
                    GetUsersListFromDB(i);
                    PeerMsgs(i);
                }
            }
        }
    }
}

void Server::HandleResponse(int clientsock) {
    Request req;
    size_t rec = recv(clientsock, &user, sizeof(user), 0);

    std::cout << "Client socket 1   " << clientsock << std::endl;

    if (rec < 0) {
        std::cerr << "Error receiving data " << clientsock << std::endl;
    } else if(rec == 0) {
        std::cout << "Client " << clientsock << " disconnected" << std::endl;
        m_database->SetOnlineStatus(user.username, false);
        store.erase(user.username);
        close(clientsock);
        FD_CLR(clientsock, &m_master);
    } else {
        if (user.start_byte == 0XCBFF){
                std::string msg = "REGISTER";
                std::string username(this->user.username);
                std::string pass(this->user.pass);

                if (m_database->CheckUser(user.username)){
                    std::cout << "This account already exists. Try to register with another username or login." << std::endl;
                    int snd = send(clientsock, &req.ERROR, sizeof(req.ERROR), 0);
                    if(snd < 0)
                        std::cout << "Failed to send ERROR reqonse for registration";
                }
                else {
                    std::cout << "stegh em 2" << std::endl;
                    Registration(user.username, clientsock);
                    send(clientsock, &req.OK, sizeof(req.OK), 0);
                }
        }

        else if(user.start_byte == 0xCBAE) {
            int i = 0;
                
            while(i <= 2){
                if (m_database->VerifyUser(user.username, user.pass)) {
                    std::cout << "Incorrect username or password. Please check and try again." << std::endl;
                    if (i == 2) {
                        std::cerr << "Maximum login attempts reached. Exiting loop..." << std::endl;
                        break;
                    } 
                    ++i;
                }
                else {
                    Login(user.username, clientsock);
                    send(clientsock, &req.OK, sizeof(req.OK), 0);
                }
                int snd = send(clientsock, &req.ERROR, sizeof(req.ERROR), 0);
                if(snd < 0)
                    std::cerr << "Failed to send ERROR response for login" << std::endl;
            }       
        }
    }
}

void Server::Registration(const std::string &user, int clientsock) {
    std::cout << "Dear " <<   this->user.username << ", you have been successfully registered." << std::endl;
    std::string username(this->user.username);
    std::string pass(this->user.pass);
    m_database->AddUser(username, pass);
    m_database->SetOnlineStatus(username, true);
    store.insert(std::make_pair(username, clientsock));
}

void Server::Login(const std::string &user, int clientsock) {
    std::cout << "Dear " << this->user.username << ", you have been logged in successfully." << std::endl;
    std::string username(this->user.username);
    std::string pass(this->user.pass);
    m_database->SetOnlineStatus(this->user.username, true);
    store.insert(std::make_pair(username, clientsock));
}

void Server::DisconnectClientFromServer() {
    std::cout << "Server is disconnecting clients..." << std::endl;

    for (int i = 0; i <= m_fdmax; ++i) {
        if (FD_ISSET(i, &m_master) && i != m_servsock) {
            std::string disconnectMsg = "SERVER_DISCONNECT";
            if (send(i, disconnectMsg.c_str(), disconnectMsg.size(), 0) < 0) {
                std::cerr << "Failed to send disconnect message to client" << std::endl;
            }
            close(i);
            FD_CLR(i, &m_master);
            m_database->SetOnlineStatus(user.username, false);
            store.erase(user.username);
        }
    }
}

void Server::ConnectionToDB(Database &database) {

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


void Server::GetUsersListFromDB(int clientsock) {
    uint16_t start_byte = 0xCAFE;

    std::cout << clientsock << std::endl;

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

void Server::PeerMsgs(int clientsocket) {
    std::string username;
    int peer_user = recv(clientsocket, &username, sizeof(username), 0);

    if (peer_user < 0) {
        std::cerr << "Failed to recevie Private message username" << std::endl;
        return ;
    }

    std::string msg;
    int peer_msg = recv(clientsocket, &msg, sizeof(msg), 0);

    if (peer_msg < 0) {
        std::cerr << "Failed to recevie Private Message msg" << std::endl;
        return ;
    }

    auto it = store.find(username);

    PacketBody(it->second, username, msg);

    int user_2 = send(it->second, &msg, sizeof(msg), 0);

    if (user_2 < 0) {
        std::cerr << "Failed to send to second user " << std::endl;
        return ;
    }
}


// void Server::AddMsgsToDB(uint32_t timestamp, std::string &username, std::string &msg) {}


void Server::UpdateMsgs() {
    updatemsg.start_byte = 0xABCF;

    
}

void Server::PacketBody(int clientsock, std::string &username, std::string &msg) {
    packet.start_byte = 0xABFD;
    packet.username_size = std::strlen(packet.username);
    packet.msg_size = std::strlen(packet.msg);
    packet.crc_checksum = sizeof(packet);

    

    auto now = std::chrono::system_clock::now();
    std::time_t current_time = std::chrono::system_clock::to_time_t(now);

    packet.timestamp = static_cast<uint32_t>(current_time);

    std::string user(packet.username);
    std::string message(packet.msg);

    user = username;
    message = msg;

    // AddMsgsToDB(packet.timestamp, user, message);
    m_database->AddMsg(packet.timestamp, user, message);
}


Server::~Server() {
    close(m_servsock);
}
