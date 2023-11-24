#include "../headers/Client.hpp"

Client::Client() {}

Client::Client(const char *port, const char *servaddr)
    : m_port(port), m_servaddr(servaddr), m_clientsock(-1), m_active(false), m_server() {}

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
        m_active = false;
    } else
        m_active = true;

    return m_active;
}

void Client::SendMsgToServer(std::string &msg){
    int snd = send(m_clientsock, &msg, sizeof(msg), 0);

    if (snd <= 0){
        if (snd == 0) {
            std::cerr << "Server disconnected " << std::endl;
            m_active = false;
        }  else {
            std::cerr << "Failed to send a message to server" << std::endl;
        }
        close(m_clientsock);
        m_clientsock = -1;
    } else {
        std::cout << "Message sent successfully" << std::endl;
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
                std::memcpy(user.username, username.c_str(), username.size());
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

bool Client::SendRegistrationRequest() {
    size_t fields = 1;
    Response resp;
    UserInfo(fields);

    if(!isActive()) {
        std::cerr << "Client is not active. Registration request not sent. " << std::endl;
        return false;
    }

    if (fields == 3) {
        user.start_byte = 0XCBFF;
        user.username_size = std::strlen(user.username);
        user.pass_size = std::strlen(user.pass);
        user.crc_checksum = sizeof(user);
    }

    int snd = send(m_clientsock, &user, sizeof(user), 0);
    if(snd < 0)
        std::cerr << "Failed to send request " << std::endl;
    else {
        uint8_t res;
        int rec = recv(m_clientsock, &res, sizeof(res), 0);
        if (res == resp.OK){
            std::cout << "You have successfully registered and logged in to your account. " << std::endl;
            m_active = true;
            return m_active;
        }
        else if(res == resp.ERROR){
            std::cout << "Failed to register " << std::endl;
            m_active = false;
            return m_active;
        }
    }
    return true;
}

bool Client::SendLoginRequest() {
    size_t fields = 1;
    int i = 0;
    Response resp;

    if(!isActive()) {
        std::cerr << "Client is not active. Login request not sent. " << std::endl;
        return false;
    }

    if(m_server.getClients().empty()) {
        std::cout << "You're not registered. Please register before it." << std::endl;
        SendRegistrationRequest();
        return false;
    }

    UserInfo(fields);

    if(fields == 3) {
        user.start_byte = 0xCBAE;
        user.username_size = std::strlen(user.username);
        user.pass_size = std::strlen(user.pass);
        user.crc_checksum = sizeof(user);
    }

    int snd = send(m_clientsock, &user, sizeof(user), 0);
    while(i < 3){
        if(snd < 0)
            std::cerr << "Failed to send request " << std::endl;
        else {
            uint8_t res;
            int rec = recv(m_clientsock, &res, sizeof(res), 0);
            if (res == resp.OK) {
                std::cout << "You are logged in successfully. " << std::endl;
                return true;
            }
            else if(res == resp.ERROR){
                std::cout << "Failed to login. Try again." << std::endl;
                ++i;
                // return false;
            }
        }
        return false;
    }
    return true;
}

// void Client::PrivateMsgs() {
//     auto it = m_server.getClients();
// }

// void Client::GroupChat() {}

// void Client::SendMsg(std::string &username, std::string &msg) {

// }

// void Client::GetAllMsgs() {
// }

Client::~Client() {
    if (m_clientsock >= 0) {
        close(m_clientsock);
        std::cout << "Disconnected successfully." << std::endl;
    }
}
