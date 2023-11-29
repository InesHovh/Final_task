#include "../headers/Client.hpp"

Client::Client() {}

Client::Client(const char *port, const char *servaddr)
    : m_port(port), m_servaddr(servaddr), m_clientsock(-1), m_active(false) {}

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

    std::cout << "M_clientsocket  " << m_clientsock << std::endl;
    if (connect(m_clientsock, result->ai_addr, result->ai_addrlen) < 0) {
        std::cerr << "Failed to connect to the server" << std::endl;
        close(m_clientsock);
        m_active = false;
    } else
        m_active = true;

    return m_active;
}

bool Client::SendMsgToServer(std::string &msg){
    int snd = send(m_clientsock, &msg, sizeof(msg), 0);

    if (snd <= 0){
        if (snd == 0) {
            std::cerr << "Server disconnected " << std::endl;
            m_active = false;
            return false;
        }  else {
            std::cerr << "Failed to send a message to server" << std::endl;
            return false;
        }
        close(m_clientsock);
        m_clientsock = -1;
    } else {
        std::cout << "Message sent successfully" << std::endl;
    }
    return true;
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

void Client::SendRegistrationRequest() {
    size_t fields = 1;
    Response resp;
    UserInfo(fields);
    
    if (fields == 3) {
        user.start_byte = 0XCBFF;
        user.username_size = std::strlen(user.username);
        user.pass_size = std::strlen(user.pass);
        user.crc_checksum = sizeof(user);
    }

    std::cout << "stegh el a mtnum " << std::endl;

    int snd = send(m_clientsock, &user, sizeof(user), 0);
    std::cout << "Sendic heto " << std::endl;
    if(snd < 0)
        std::cerr << "Failed to send request " << std::endl;
    else {
        uint8_t res;
        int rec = recv(m_clientsock, &res, sizeof(res), 0);
        std::cout << "stegh em " << std::endl;
        if (res == resp.OK){
            std::cout << "You have successfully registered and logged in to your account. " << std::endl;
            return ;
        }
        else if(res == resp.ERROR){
            std::cout << "Failed to register " << std::endl;
            return ;
        }
    }
}

void Client::SendLoginRequest() {
    size_t fields = 1;
    int i = 0;
    Response resp;

    UserInfo(fields);

    if(fields == 3) {
        user.start_byte = 0xCBAE;
        user.username_size = std::strlen(user.username);
        user.pass_size = std::strlen(user.pass);
        user.crc_checksum = sizeof(user);
    }

    int snd = send(m_clientsock, &user, sizeof(user), 0);
    if(snd < 0)
        std::cerr << "Failed to send request " << std::endl;
    while(i < 3){
        uint8_t res;
        int rec = recv(m_clientsock, &res, sizeof(res), 0);
        if (res == resp.OK) {
            std::cout << "You are logged in successfully. " << std::endl;
            return ;
        }
        else if(res == resp.ERROR){
            std::cout << "Failed to login. Try again." << std::endl;
            // send(m_clientsock, &user, sizeof(user), 0);
            return ;
        }
    }
}

void Client::GetUsersList() {
    uint16_t start_byte = 0xCAFE;


    int snd = send(m_clientsock, &start_byte, sizeof(start_byte), 0);
    std::cout << "yes aystegh em hima " << std::endl;

    if (snd < 0) {
        std::cerr << "Failed to send start_byte to server" << std::endl;
        return;
    }

    uint16_t numUsers;
    size_t rec = recv(m_clientsock, &numUsers, sizeof(numUsers), 0);

    if (rec < 0) {
        std::cerr << "Error receiving number of users from server" << std::endl;
        return;
    }

    for (uint16_t i = 0; i < numUsers; ++i) {
        size_t len;
        rec = recv(m_clientsock, &len, sizeof(len), 0);
        if (rec < 0) {
            std::cerr << "Error receiving username length from server" << std::endl;
            return;
        } else if(rec == 0) {
            close(m_clientsock);
        }

        char buffer[len + 1];
        buffer[len] = '\0';

        rec = recv(m_clientsock, buffer, len, 0);

        if (rec < 0) {
            std::cerr << "Error receiving username from server" << std::endl;
            return;
        } else if(rec == 0) {
            close(m_clientsock);
        }

        std::cout << "User " << i + 1 << "  " << buffer << std::endl;
    }
}



bool Client::SendToSecondUser(const std::string &user, const std::string &msg) {
    size_t size = user.size();
    send(m_clientsock, &size, sizeof(size), 0);

    size_t snd1 = send(m_clientsock, user.c_str(), size, 0);

    if (snd1 < 0) {
        std::cerr << "Failed to send in Private Message username" << std::endl;
        return false;
    }

    while(1) {
        size_t msg_size = msg.size();
        send(m_clientsock, &msg_size, sizeof(msg_size), 0);

        size_t snd2 = send(m_clientsock, msg.c_str(), msg_size, 0);

        if (snd2 < 0) {
            std::cerr << "Failed to send in Private Message message" << std::endl;
            return false;
        }

        size_t msg_size1;
        size_t rec = recv(m_clientsock, &msg_size1, sizeof(msg_size1), 0);

        if (rec < 0) {
            std::cerr << "Failed to recevie Private msg_size" << std::endl;
            return false;
        }

        std::string msg1(msg_size1, ' ');
        size_t rec1 = recv(m_clientsock, &msg1[0], msg_size1, 0);

        if (rec1 < 0) {
            std::cerr << "Failed to recevie Private msg1" << std::endl;
            return false;
        }

    }
    return true;
}

// bool Client::ReceiveToTheFirst(std::string &response){
//     size_t response_size;
//     size_t rec = recv(m_clientsock, &response_size, sizeof(response_size), 0);

//     if(rec < 0){
//         std::cerr << "Failed to receive " << std::endl;
//         return false;
//     }

//     response.resize(response_size + 1);
//     size_t rec_size = recv(m_clientsock, &response[0], response_size, 0);

//     if(rec_size < 0){
//         std::cerr << "Failed to receive " << std::endl;
//         return false;
//     }
//     return true;
// }


void Client::GetAllMsgs() {
    uint16_t start_byte = 0xCBED;

    int snd = send(m_clientsock, &start_byte, sizeof(start_byte), 0);

    if (snd < 0)
        std::cerr << "Failed to send All Messages request " << std::endl;

    
}

Client::~Client() {
    if (m_clientsock >= 0) {
        close(m_clientsock);
        std::cout << "Disconnected successfully." << std::endl;
    }
}
