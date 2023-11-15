#include <iostream>
#include "Client.hpp"

Client::Client(const char *port, const char *servaddr)
    : m_port(port), m_servaddr(servaddr), m_clientsock(-1), m_login(), m_pass() {}

bool Client::Start() {
    struct addrinfo clientaddr, *result;

    memset(&clientaddr, 0, sizeof(clientaddr));
    clientaddr.ai_family = AF_UNSPEC;
    clientaddr.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(m_servaddr, m_port, &clientaddr, &result) < 0) {
        std::cerr << "getaddrinfo error" << std::endl;
        return false;
    }

    m_clientsock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_clientsock < 0) {
        std::cerr << "Failed to create the socket" << std::endl;
        freeaddrinfo(result);
        return false;
    }

    if (connect(m_clientsock, result->ai_addr, result->ai_addrlen) < 0) {
        std::cerr << "Failed to connect to the server" << std::endl;
        close(m_clientsock);
        freeaddrinfo(result);
        return false;
    }

    freeaddrinfo(result);
    return true;
}

bool Client::SendMsgToServer(const std::string &msg) {
    if (send(m_clientsock, msg.c_str(), msg.size(), 0) < 0) {
        std::cerr << "Failed to send a message" << std::endl;
        return false;
    }
    return true;
}

bool Client::ReceiveMsgFromServer(std::string &receivedMsg) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // std::cout << m_clientsock << std::endl;
    int rec = recv(m_clientsock, buffer, sizeof(buffer), 0);
    // std::cout << rec << std::endl;
    if (rec < 0) {
        std::cerr << "Failed to receive a message" << std::endl;
        return false;
    } else if (rec == 0) {
        std::cout << "Server closed the connection" << std::endl;
        close(m_clientsock);
        return false;
    }
    receivedMsg = buffer;
    return true;
}

Client::~Client() {
    if (m_clientsock >= 0) {
        close(m_clientsock);
        std::cout << "Disconnected successfully." << std::endl;
    }
}

bool Client::SignUp() {
    std::string reg = "Sign Up " + m_login + m_pass;
    return SendMsgToServer(reg);
}

bool Client::SignIn() {
    std::string log = "Sign In " + m_login + m_pass;
    return SendMsgToServer(log);
}

int main() {
    Client client("1245", "127.0.0.1");
    if (client.Start()) {
        std::cout << "Connected successfully" << std::endl;

        // client.SignUp();
        // client.SignIn();

        while (true) {
            std::string msg;
            std::string receivedMsg;
            std::cout << "Enter the message to send " << std::endl;
            std::getline(std::cin, msg);
            
            if (!client.SendMsgToServer(msg)) {
                std::cout << "Disconnecting " << std::endl;
                break;
            }
            if (!client.ReceiveMsgFromServer(receivedMsg)) {
                std::cout << "Disconnected from server" << std::endl;
                break;
            }

            std::cout << "Received from server: " << receivedMsg << std::endl;
        }
    }

    return 0;
}