#include <iostream>
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
    // while(1) {
        if (send(m_clientsock, msg.c_str(), msg.size(), 0) < 0) {
            std::cerr << "Failed to send a message" << std::endl;
            return false;
        }
        return true;
    // }
}

bool Client::ReceiveMsgFromServer(std::string &receivedMsg) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int rec = recv(m_clientsock, buffer, sizeof(buffer), 0);
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

std::string Client::check_empty_line(std::string str)
{
	while (str.length() == 0)
	{
		std::cout << "Empty line!\nEnter valid data: ";
		std::getline(std::cin, str);
	}
	return (str);
}


std::string Client::LoginRequest(const std::string &request) {
    std::array<uint8_t, 516> arr{};

    auto buff = arr.begin();
    User user;
    user.start_byte = 0xCBAE;

    memcpy(buff, &user.start_byte, sizeof(user.start_byte));
    buff += sizeof(user.start_byte);

    memcpy(buff, &user.username_size, sizeof(user.username_size));
    buff += sizeof(user.username_size);

    memset(user.username, 0, sizeof(user.username));
    memcpy(buff, user.username, sizeof(user.username));
    buff += sizeof(user.username);

    memcpy(buff, &user.pass_size, sizeof(user.pass_size));
    buff += sizeof(user.pass_size);

    memset(user.pass, 0, sizeof(user.pass));
    memcpy(buff, user.pass, sizeof(user.pass));
    buff += sizeof(user.pass);

    memcpy(buff, &user.crc_checksum, sizeof(user.crc_checksum));
    buff += sizeof(user.crc_checksum);

    std::string result(arr.begin(), arr.end());
    return result;
}

std::string Client::RegRequest(const std::string &request) {
    if (request.size() != 516) {
        return "ERROR";
    }

    User user;

    std::memcpy(&user.start_byte, request.data(), sizeof(user.start_byte));
    std::memcpy(&user.username_size, request.data() + sizeof(user.start_byte), sizeof(user.username_size));

    size_t usernameoffset = sizeof(user.start_byte) + sizeof(user.username_size);
    std::memcpy(user.username, request.data() + usernameoffset, user.username_size);
    user.username[user.username_size] = '\0';

    size_t passoffset = usernameoffset + user.username_size;
    std::memcpy(&user.pass_size, request.data() + passoffset, sizeof(user.pass_size));

    passoffset += sizeof(user.pass_size);
    std::memcpy(user.pass, request.data() + passoffset, user.pass_size);
    user.pass[user.pass_size] = '\0';

    size_t crcoffset = passoffset + user.pass_size;
    std::memcpy(&user.crc_checksum, request.data() + crcoffset, sizeof(user.crc_checksum));

    // std::string response ;
    return request;
}

uint8_t Client::LoginResponse() {
    Response resp;
    uint8_t buff[2];
    size_t bytes = recv(m_clientsock, buff, sizeof(buff), 0);

    if (bytes == sizeof(buff)) {
        if (buff[0] == resp.OK) {
            std::cout << "Login was successful " << std::endl;
            return buff[0];
        } else {
            buff[1] = resp.ERROR;
            std::cout << "Failed to login " << std::endl;
            return buff[1];
        }
    } else {
        std::cerr << "Error " << std::endl;
        return 0xFF;
    }
}

uint8_t Client::RegResponse() {
    Response resp;
    uint8_t buff[2];
    size_t bytes = recv(m_clientsock, buff, sizeof(buff), 0);

    if (bytes == sizeof(buff)) {
        if (buff[0] == resp.OK) {
            std::cout << "Registration was successful " << std::endl;
            return buff[0];
        } else {
            buff[1] = resp.ERROR;
            std::cout << "Failed to register " << std::endl;
            return buff[1];
        }
    } else {
        std::cerr << "Error " << std::endl;
        return 0xFF;
    }
}

void Client::AddUser() {
    Client client;
    std::string str;

    std::cout << "Username: " << std::endl;
    std::getline(std::cin, str);
    str = check_empty_line(str);
    client.setUsername(str);

    std::cout << "Password: " << std::endl;
    std::getline(std::cin, str);
    str = check_empty_line(str);
    client.setPass(str);

    // this->clients.insert(std::make_pair(client.getUsername(), client.getPass()));
}

void Client::SignUp() {
    AddUser();

    std::string regrequest;

    if (SendMsgToServer(regrequest)) {
        uint8_t regresp = RegResponse();
        Response resp;
        if (regresp == resp.OK)
            std::cout << "Successful registration " << std::endl;
        else
            std::cout << "Registration failed " << std::endl;
    } else
        std::cout << "Failed to send request " << std::endl;
}

void Client::SignIn() {
    AddUser();

    std::string loginreq;

    if (SendMsgToServer(loginreq))
        uint8_t regresp = LoginResponse();
    else
        std::cout << "Failed to send request " << std::endl;
}

Client::~Client() {
    if (m_clientsock >= 0) {
        close(m_clientsock);
        std::cout << "Disconnected successfully." << std::endl;
    }
}