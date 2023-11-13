#include <iostream>
#include "Server.hpp"

// Server::Server() {}

Server::Server(const char *port) : m_port(port){
    struct addrinfo servaddr, *result;

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.ai_family = AF_UNSPEC;
    servaddr.ai_socktype = SOCK_STREAM;
    servaddr.ai_flags = AI_PASSIVE;

    if (getaddrinfo(nullptr, this->m_port.c_str(), &servaddr, &result) < 0) {
        std::cerr << "getaddrinfo error" << std::endl;
        exit(EXIT_FAILURE);
    }

    this->m_servsock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (this->m_servsock < 0){
        std::cerr << "Failed to create the socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (bind(this->m_servsock, result->ai_addr, result->ai_addrlen) < 0){
        std::cerr << "Failed to bind " << std::endl;
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);
}

void Server::Start() {
    if (listen(this->m_servsock, 10) < 0){
        std::cerr << "Failed to listen " << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << " Server started. Waiting for connections " << std::endl;

    while (1) {
        int clientsock = accept(this->m_servsock, nullptr, nullptr);
        if (clientsock < 0){
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }
        setClient(clientsock);
        close(clientsock);
    }
}

// void Server::Stop(){

// }

// int Server::getPort() const { return m_port; }

// std::string Server::getPass() const { return m_pass; }

// Client *Server::getClient() const {}

void Server::setClient(int clientsock) {
    char buff[1024];
    int bytes;

    while ((bytes = recv(clientsock, buff, sizeof(buff), 0)) > 0){
        buff[bytes] = '\0';
        std::cout << "Received from client: " << buff << std::endl;
        send(clientsock, buff, bytes, 0);
    }
    if (bytes == 0)
        std::cout << "Client disconnected" << std::endl;
    else if(bytes < 0)
        std::cerr << "Failed to receive " << std::endl;
}

void Server::NewClient() {}

void Server::DeleteClient() {}

void Server::Registration() {}

void Server::AlreadyExist() {}

// void Server::AddToDB() {}

// void Server::PutFromDB() {}

Server::~Server() {
    close(this->m_servsock);
}


int main() {
    Server server("8080");

    server.Start();
}