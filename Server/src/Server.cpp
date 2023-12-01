#include "../headers/Server.hpp"

Server::Server(const char *port, Database *database) : acceptedSocketsCount(0)
{
    this->database = database;
    init(port);
    ConnectionToDB(*this->database);
    start();
}

void Server::init(const char *port)
{
    struct addrinfo hints, *result, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(nullptr, port, &hints, &result) != 0)
    {
        std::cerr << "getaddrinfo error" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (p = result; p != nullptr; p = p->ai_next)
    {
        serverSocketFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (serverSocketFD == -1)
            continue;

        if (bind(serverSocketFD, p->ai_addr, p->ai_addrlen) == 0)
            break;

        close(serverSocketFD);
    }

    if (p == nullptr)
    {
        std::cerr << "Failed to bind" << std::endl;
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    fcntl(serverSocketFD, F_SETFL, O_NONBLOCK);

    int listenResult = listen(serverSocketFD, 10);
    if (listenResult == -1)
    {
        std::cerr << "Failed to listen" << std::endl;
        close(serverSocketFD);
        exit(EXIT_FAILURE);
    }
}

void Server::start()
{
    std::cout << "Server started. Waiting for connections..." << std::endl;

    while (true)
    {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(serverSocketFD, &read_fds);

        timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 300000;

        if (select(serverSocketFD + 1, &read_fds, nullptr, nullptr, &timeout) == -1)
        {
            std::cerr << "select error" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(serverSocketFD, &read_fds))
        {
            AcceptedSocket *clientSocket = acceptIncomingConnection();
            acceptedSockets[acceptedSocketsCount++] = *clientSocket;

            pthread_t id;
            pthread_create(&id, nullptr, receiveAndPrintIncomingData, this);
        }
    }
}


void *Server::receiveAndPrintIncomingData(void *data)
{
    Server *server = static_cast<Server *>(data);
    server->receiveIncomingData(server->acceptedSockets[server->acceptedSocketsCount - 1].acceptedSocketFD);
    return nullptr;
}

void Server::receiveIncomingData(int socketFD)
{
    char buffer[2048];
    uint8_t OK = 0x01, ERROR = 0x00;
    int i = 0;

    while (true)
    {
        ssize_t amountReceived = recv(socketFD, buffer, 1024, 0);

        if (i == 0)
        {
            std::string combine(buffer);

            size_t split = combine.find(":");
            std::string name = combine.substr(0, split);
            std::string pass = combine.substr(split + 1);

            bool check = database->CheckUser(name);
            if (!check)
            {
                std::cout << "You have successfully registered." << std::endl;
                database->AddClient(name, pass);
                continue;
            }
            else
            {
                bool loginSuccess = database->VerifyUser(name, pass);
                if (loginSuccess)
                {
                    std::cout << "You have successfully logged in." << std::endl;
                }
                else
                {
                    std::cout << "Incorrect username or password. Try again later. " << std::endl;
                }
            }
            ++i;
        }

        if (amountReceived > 0)
        {
            std::string combine(buffer);
            size_t msgs = combine.find(":");
            std::string start_byte = combine.substr(0, msgs);
            std::string username = combine.substr(msgs + 1);
            buffer[amountReceived] = 0;

            if (start_byte == std::to_string(0xCBFD))
            {
                std::vector<std::string> messages = database->ShowMessages(username);

                std::string allMessages;
                for (const auto &msg : messages)
                {
                    allMessages += msg + ";";
                }

                send(socketFD, allMessages.c_str(), allMessages.size(), 0);
                continue;
            }
            SenddMessageToTheClients(buffer, socketFD);
        }
        else if (amountReceived == 0)
        {
            close(socketFD);
            return;
        }

        if (i > 0)
        {
            std::istringstream str(buffer);
            std::string first, second;

            std::getline(str, first, ':');
            std::getline(str >> std::ws, second);

            PacketBody(first, second);
        }

        if (amountReceived == 0)
            break;
    }

    close(socketFD);
}



void Server::PacketBody(const std::string &name, const std::string &msg) {
    packet.name = name;
    packet.msg = msg;


    std::time_t current_time = std::time(nullptr);
    packet.timestamp = std::ctime(&current_time);

    database->AddMsg(packet.timestamp, packet.name, packet.msg);
}

void Server::ConnectionToDB(Database &database) {

    try
    {
        if (database.ConnectionToServer()) {
            std::cout << "Server connected to DB" << std::endl;
        } else {
            throw std::runtime_error("Failed to connect to DB");
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error " << e.what() << std::endl;
    }
}

void Server::SenddMessageToTheClients(char *buffer, int senderSocketFD)
{
    for (int i = 0; i < acceptedSocketsCount; i++)
        if (acceptedSockets[i].acceptedSocketFD != senderSocketFD)
        {
            send(acceptedSockets[i].acceptedSocketFD, buffer, strlen(buffer), 0);
        }
}

Server::AcceptedSocket *Server::acceptIncomingConnection()
{
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverSocketFD, (struct sockaddr *)&clientAddress, &clientAddressSize);

    AcceptedSocket *acceptedSocket = new AcceptedSocket;
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFD = clientSocketFD;
    acceptedSocket->acceptedSuccessfully = clientSocketFD > 0;

    if (!acceptedSocket->acceptedSuccessfully)
        acceptedSocket->error = clientSocketFD;

    return acceptedSocket;
}

void Server::startAccepting()
{
    while (true)
    {
        AcceptedSocket *clientSocket = acceptIncomingConnection();
        acceptedSockets[acceptedSocketsCount++] = *clientSocket;

        pthread_t id;
        pthread_create(&id, nullptr, receiveAndPrintIncomingData, this);
    }
}

Server::~Server()
{
    shutdown(serverSocketFD, SHUT_RDWR);
}
