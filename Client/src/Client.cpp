#include "../headers/Client.hpp"

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
    }

    m_clientsock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_clientsock < 0) {
        std::cerr << "Failed to create the socket" << std::endl;
    }

    if (connect(m_clientsock, result->ai_addr, result->ai_addrlen) < 0) {
        std::cerr << "Failed to connect to the server" << std::endl;
        close(m_clientsock);
    }

    return true;
}

void Client::SendMsgToServer(std::string &msg){
    int snd = send(m_clientsock, &msg, sizeof(msg), 0);

    if (snd < 0) {
        std::cerr << "Failed to send a message to server" << std::endl;
    }  else {
        std::cout << "stegh em " << std::endl;
    }
}

void Client::UserInfo(size_t &fields) {
    while (fields <= 2) {
        if (std::cin.eof())
            break;
        if (fields == 1) {
            std::cout << "Please Enter Your Username: " << std::endl;
            std::string username;
            std::getline(std::cin, username);
            if (!username.empty()) {
                // std::cout << "Username:  " << username << "   ####" << std::endl;
                std::memcpy(user.username, username.c_str(), username.size());
                // std::cout << "New Username:  " << user.username << "    *******" << std::endl;
                fields++;
            }
        }
        if (fields == 2) {
            std::string pass;
            std::cout << "Please Choose Your Password: " << std::endl;
            std::getline(std::cin, pass);
            if (!pass.empty()) {
                std::memcpy(user.pass, pass.c_str(), pass.size());
                fields++;
            }
        }
    }
}

void Client::Response() {

}

bool Client::SendRegistrationRequest() {
    size_t fields = 1;
    UserInfo(fields);
    
    std::cout << fields << std::endl;

    if (fields == 3) {
        user.start_byte = 0XCBFF;
        user.username_size = std::strlen(user.username);
        user.pass_size = std::strlen(user.pass);
        user.crc_checksum = sizeof(user);
        std::cout << user.username << " : " << user.pass << std::endl;
    }

    int snd = send(m_clientsock, &user, sizeof(user), 0);
    if(snd < 0)
        std::cerr << "Failed to send request " << std::endl;
    // else 
    //     std::cout << "success " << std::endl;

    return true;
}



bool Client::SendLoginRequest() {
    size_t fields = 1;
    UserInfo(fields);

    // std::memset(&user, 0, sizeof(user));
    if(fields == 3) {
        user.start_byte = 0xCBAE;
        user.username_size = std::strlen(user.username);
        user.pass_size = std::strlen(user.pass);
        user.crc_checksum = sizeof(user);
        std::cout << user.username << " : " << user.pass << std::endl;
    }

    int snd = send(m_clientsock, &user, sizeof(user), 0);
    if(snd < 0)
        std::cerr << "Failed to send request " << std::endl;
    // else 
    //     std::cout << "success " << std::endl;

    return true;
}

Client::~Client() {
    if (m_clientsock >= 0) {
        close(m_clientsock);
        std::cout << "Disconnected successfully." << std::endl;
    }
}
