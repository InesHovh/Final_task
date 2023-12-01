#include "../headers/Client.hpp"

Client::Client() {
    socketFD = createTCPIpv4Socket();
    struct sockaddr_in *address = createIPv4Address("127.0.0.1", 1245);

    int result = connect(socketFD, reinterpret_cast<const sockaddr*>(address), sizeof(*address));

    if (result == 0)
        std::cout << "Connection was successful\n";
}

int Client::createTCPIpv4Socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

struct sockaddr_in* Client::createIPv4Address(const char* ip, int port) {
    struct sockaddr_in *address = new struct sockaddr_in;
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if (strlen(ip) == 0)
        address->sin_addr.s_addr = INADDR_ANY;
    else
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);

    return address;
}

void Client::start() {
    startListeningAndPrintMessagesOnNewThread();
    readConsoleEntriesAndSendToServer();
}

void *Client::listenAndPrintWrapper(void *data) {
    Client *client = static_cast<Client*>(data);
    client->listenAndPrint();
    return nullptr;
}

void Client::listenAndPrint() {
    char buffer[1024];

    while (true) {
        ssize_t amountReceived = recv(socketFD, buffer, 1024, 0);

        if (amountReceived > 0) {
            buffer[amountReceived] = 0;
            std::cout << buffer << std::endl;
        }

        if (amountReceived == 0)
            break;
    }
}

void Client::startListeningAndPrintMessagesOnNewThread() {
    pthread_t id;
    pthread_create(&id, nullptr, listenAndPrintWrapper, this);
}

void Client::readConsoleEntriesAndSendToServer() {
    char *name = nullptr, *pass = nullptr;
    size_t nameSize = 0, passSize = 0;
    std::cout << "Please enter your username" << std::endl;
    ssize_t nameCount = getline(&name, &nameSize, stdin);
    name[nameCount - 1] = 0;

    std::cout << "Please enter your password" << std::endl;
    ssize_t passCount = getline(&pass, &passSize, stdin);
    pass[passCount - 1] = 0;

    std::string combine = name + std::string(":") + pass;

    send(socketFD, combine.c_str(), sizeof(combine), 0);

    char *line = nullptr;
    size_t lineSize = 0;
    std::cout << "Type the message..." << std::endl;
    std::cout << std::endl;
    std::cout << "If you want to see your all time messages please type 'show'" << std::endl;
    std::cout << std::endl;

    char buffer[1024];

    while (true) {
        ssize_t charCount = getline(&line, &lineSize, stdin);
        line[charCount - 1] = 0;

        std::string username(name);

        if(strcmp(line, "show") == 0){
            showMsgs(socketFD, username);
            receiveAndPrintAllMessages(socketFD);
            continue;
        }

        sprintf(buffer, "%s: %s", name, line);

        if (charCount > 0) {
            if (strcmp(line, "exit") == 0)
                break;

            ssize_t amountWasSent = send(socketFD, buffer, strlen(buffer), 0);
        }
    }
}

void Client::showMsgs(int socketFD, std::string &name) {
    uint16_t start_byte = 0xCBFD;

    std::string mix = std::to_string(start_byte) + ":" + name;

    send(socketFD, mix.c_str(), sizeof(mix), 0);
}

void Client::receiveAndPrintAllMessages(int socketFD)
{
    char buffer[2048];

    ssize_t amountReceived = recv(socketFD, buffer, sizeof(buffer), 0);

    if (amountReceived > 0)
    {
        buffer[amountReceived] = '\0';
        std::istringstream messageStream(buffer);
        std::string individualMessage;
        while (std::getline(messageStream, individualMessage, ';'))
        {
            std::cout << "Received message: " << individualMessage << std::endl;
        }
    }
    else
    {
        std::cout << "Failed to receive messages from server." << std::endl;
    }
}


Client::~Client() {
    close(socketFD);
}
