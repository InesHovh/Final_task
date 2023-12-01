#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <cstring>
#include <thread>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <arpa/inet.h>
#include <sstream>

class Client {
public:

private:
    int socketFD;
public:
    Client();
    ~Client();

    void start();
    static int createTCPIpv4Socket();
    static sockaddr_in* createIPv4Address(const char* ip, int port);
    static void *listenAndPrintWrapper(void *data);
    void listenAndPrint();
    void readConsoleEntriesAndSendToServer();
    void showMsgs(int socketFD, std::string &name);
    void receiveAndPrintAllMessages(int socketFD);
    void startListeningAndPrintMessagesOnNewThread();
};

#endif // CLIENT_HPP
