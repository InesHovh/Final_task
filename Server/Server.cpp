#include <iostream>
#include "Server.hpp"

Server::Server() {}

Server::Server(int port, std::string &pass) : m_port(port), m_pass(pass) {
    Start();
}

void Server::Start() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
        std::cerr << "Failed to create the socket" << std::endl;
    
    int opt;
}

int Server::getPort() const { return m_port; }

std::string Server::getPass() const { return m_pass; }

Client *Server::getClient() const {}

void Server::setClient(Client *client, std::string &nick, int fd) {}

void Server::NewClient() {}

void Server::DeleteClient() {}

void Server::Registration() {}

void Server::AlreadyExist() {}

void Server::AddToDB() {}

void Server::PutFromDB() {}

Server::~Server() {}


int main() {
    std::cout << "Hiii Server" << std::endl;
}