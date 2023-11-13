#include <iostream>
#include "Client.hpp"

Client::Client(const char *port, const char *servaddr)
 : m_port(port), m_servaddr(servaddr), m_loggedin(false), m_clientsock(-1) {
}

bool Client::Start() {
    struct addrinfo clientaddr, *result;

    memset(&clientaddr, 0, sizeof(clientaddr));
    clientaddr.ai_family = AF_UNSPEC;
    clientaddr.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(this->m_servaddr.c_str(), this->m_port.c_str(), &clientaddr, &result) < 0) {
        std::cerr << "getaddrinfo error" << std::endl;
        return false;
        // exit(EXIT_FAILURE);
    }

    this->m_clientsock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (this->m_clientsock < 0) {
        std::cerr << "Failed to create the socket" << std::endl;
        freeaddrinfo(result);
        return false;
        // exit(EXIT_FAILURE);
    }

    if (connect(this->m_clientsock, result->ai_addr, result->ai_addrlen) < 0) {
        std::cerr << "Failed to connect to the server" << std::endl;
        close(this->m_clientsock);
        freeaddrinfo(result);
        // exit(EXIT_FAILURE);
        return false;
    }
    freeaddrinfo(result); 
    return true;
}

void Client::Disconnection() {
    if (this->m_clientsock < 0) {
        close(this->m_clientsock);
        this->m_clientsock = -1;
    }
}

bool Client::SignUp(const std::string& name, const std::string& nick, const std::string& pass) {
    
}

bool Client::SignIn(const std::string& nick, const std::string& pass) {
    if (!nick.empty() && !pass.empty()){
        char *message = "Sign In successful. ";
        this->m_loggedin = true;
        send(this->m_clientsock, message, strlen(message), 0);
    } else {
        char *message = "Nickname not found. Please register";
        send(this->m_clientsock, message, strlen(message), 0);
    }

}

// int Client::getPort() const { return m_port; }

// std::string Client::getName() const { return m_name; }

// std::string Client::getNick() const { return m_nick; }

// std::string Client::getPass() const { return m_pass; }

// std::string Client::getMsg() const { return m_msg; }

// void Client::setName(std::string &name) { m_name = name; }

// void Client::setNick(std::string &nick) { m_nick = nick; }

// void Client::setPass(std::string &pass) { m_pass = pass; }

// void Client::setMsg(std::string &msg) { m_msg = msg; }

bool Client::SendMsg(std::string &nick, std::string &msg) {
    std::string buf = nick + msg + "\r\n";

    if (send(this->m_clientsock, buf.c_str(), sizeof(buf), 0) < 0){
        std::cerr << "Failed to send a message " << std::endl;
        close(this->m_clientsock);
        return false;
    } 
    return true;
}

// void Client::PrivateChat() {}

Client::~Client() {
    Disconnection();
}

int main() {
    Client client("8080", "127.0.0.1");

    if(client.Start()) {
        std::string nick = "Harry ";
        std::string msg = "Hello";
        client.SendMsg(nick, msg);
        client.Disconnection();
    }else{
        std::cerr << "Failed to connect" << std::endl;
        return 1;
    }
}