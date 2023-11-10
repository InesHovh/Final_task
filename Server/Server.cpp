#include <iostream>
#include "Server.hpp"

Server::Server() {}

Server::Server(int port, std::string &pass) : m_port(port), m_pass(pass) {
    start();
}

void Server::start() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
        std::cerr << "Failed to create the socket" << std::endl;
    
    
}

int Server::getport() const {}

std::string Server::getpass() const {}

Client *Server::getclient() const {}

void Server::setclient(Client *client, std::string &nick, int fd) {}

void Server::newclient() {}

void Server::deleteclient() {}

Server::~Server() {}


int main() {
    std::cout << "Hiii Server" << std::endl;
}