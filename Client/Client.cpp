#include <iostream>
#include "Client.hpp"

Client::Client() {}

Client::Client(int port, std::string &name, std::string &nick,
        std::string &pass) : m_port(port), m_name(name), m_nick(nick), m_pass(pass) {

    m_msg = "";
}

void Client::Run() {}

void Client::SignUp() {}

void Client::SignIn() {}

int Client::getPort() const { return m_port; }

std::string Client::getName() const { return m_name; }

std::string Client::getNick() const { return m_nick; }

std::string Client::getPass() const { return m_pass; }

std::string Client::getMsg() const { return m_msg; }

void Client::setName(std::string &name) { m_name = name; }

void Client::setNick(std::string &nick) { m_nick = nick; }

void Client::setPass(std::string &pass) { m_pass = pass; }

void Client::setMsg(std::string &msg) { m_msg = msg; }

void Client::SendMsg(std::string &nick, std::string &msg) {}

void Client::PrivateChat() {}

Client::~Client() {}

int main() {
    std::cout << "Hiii Client" << std::endl;
}