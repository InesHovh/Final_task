#include "Server.hpp"

Server::Server(const char *port) : m_servsock(-1) {
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
        if (select(m_fdmax + 1, &rd, nullptr, nullptr, nullptr) == -1) {
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
                    char buffer[1024];
                    memset(buffer, 0, sizeof(buffer));
                    int rec = recv(i, buffer, sizeof(buffer) - 1, 0);

                    if (rec < 0) {
                        std::cerr << "Failed to receive a message" << std::endl;
                        close(i);
                        FD_CLR(i, &m_master);
                    } else if (rec == 0) {
                        std::cout << "Client disconnected" << std::endl;
                        close(i);
                        FD_CLR(i, &m_master);
                    } else {
                        std::cout << "Received from client: " << buffer << std::endl;

                        std::string response = "Server received: " + std::string(buffer);
                        if (send(i, response.c_str(), response.size(), 0) < 0) {
                            std::cerr << "Failed to send a message" << std::endl;
                            close(i);
                            FD_CLR(i, &m_master);
                        }
                    }
                }
            }
        }
    }
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
    std::string dbname = "mydb";
    std::string user = "ines";
    std::string password = "pass";
    std::string host = "127.0.0.1";
    std::string port = "5432";

    if (database.ConnectionToServer()) {
        std::cout << "Server connected to DB" << std::endl;
    }else
        std::cerr << "Failed to connect to DB" << std::endl;
}


// void Server::SignUp() {}

// void Server::SignIn() {}

// void Server::AddToDB() {}

// void Server::PutFromDB() {}

Server::~Server() {
    close(m_servsock);
}

int main() {
    Server server("1245");
    Database database("mydb", "ines", "pass", "localhost", "5432");
    // server.Start();

    server.ConnectionToDB(database);
    database.CreateUsersTable();
    database.CreateMsgsTable();

    // server.DisconnectClient();
    return 0;
}

