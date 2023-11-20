#include "Client.hpp"

Client::Client() {}

Client::Client(const char *port, const char *servaddr)
    : m_port(port), m_servaddr(servaddr), m_clientsock(-1) {}

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

bool Client::SendMsgToServer(const char *msg, size_t size) {
    if (send(m_clientsock, msg, size, 0) < 0) {
        std::cerr << "Failed to send a message" << std::endl;
        return false;
    }
    return true;
}

bool Client::ReceiveMsgFromServer(char *buffer, size_t size) {
    memset(buffer, 0, size);

    int rec = recv(m_clientsock, buffer, size - 1, 0);
    if (rec < 0) {
        std::cerr << "Failed to receive a message" << std::endl;
        return false;
    } else if (rec == 0) {
        std::cout << "Server closed the connection" << std::endl;
        close(m_clientsock);
        return false;
    }
    return true;
}

bool Client::SendLoginRequest() {
    user.start_byte = 0xCBAE;
    user.username_size = getUsername().size();
    user.pass_size = getPass().size();
    user.crc_checksum = 0;

    if (!SendMsgToServer(reinterpret_cast<char*>(&user), sizeof(User))) {
        return false;
    }

    return true;
}

bool Client::SendRegistrationRequest() {
    user.start_byte = 0xCBFF;
    user.username_size = getUsername().size();
    user.pass_size = getPass().size();
    user.crc_checksum = 0;

    if (!SendMsgToServer(reinterpret_cast<char*>(&user), sizeof(User))) {
        return false;
    }

    return true;
}

bool Client::ReceiveResponse(Response &response) {
    char buffer[sizeof(Response)];
    if (!ReceiveMsgFromServer(buffer, sizeof(Response))) {
        return false;
    }

    response = *reinterpret_cast<Response*>(buffer);
    return true;
}

std::string Client::check_empty_line(std::string str) {
    while (str.length() == 0) {
        std::cout << "Empty line!\nEnter valid data: ";
        std::getline(std::cin, str);
    }
    return (str);
}

void Client::AddUser() {
    std::string str;

    std::cout << "Username: " << std::endl;
    std::getline(std::cin, str);
    str = check_empty_line(str);
    setUsername(str);

    std::cout << "Password: " << std::endl;
    std::getline(std::cin, str);
    str = check_empty_line(str);
    setPass(str);
}

Client::~Client() {
    if (m_clientsock >= 0) {
        close(m_clientsock);
        std::cout << "Disconnected successfully." << std::endl;
    }
}
