#include <iostream>
#include "Client.hpp"

Client::Client() {}

Client::Client(int port, std::string &name, std::string &nick,
        std::string &pass) : m_port(port), m_name(name), m_nick(nick), m_pass(pass) {}

void Client::run() {}

int Client::getport() const {}

std::string Client::getname() const {}

std::string Client::getnick() const {}

std::string Client::getpass() const {}

void Client::setname(std::string &name) {}

void Client::setnick(std::string &nick) {}

void Client::setpass(std::string &pass) {}

Client::~Client() {}

int main() {
    std::cout << "Hiii Client" << std::endl;
}