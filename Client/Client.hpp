#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Client
{
private:
    int m_port;
    
    std::string m_name;
    std::string m_nick;
    std::string m_pass;
    std::string m_msg;

    bool m_reg;

    int m_clientfd;
    struct sockaddr_in m_clientaddr;
public:
    Client();
    Client(int port, std::string &name, std::string &nick,
            std::string &pass);
    
    void Run();

    void SignUp();
    void SignIn();

    int getPort() const;
    std::string getName() const;
    std::string getNick() const;
    std::string getPass() const;

    std::string getMsg() const;

    void setName(std::string &name);
    void setNick(std::string &nick);
    void setPass(std::string &pass);

    void setMsg(std::string &msg);
    void SendMsg(std::string &nick, std::string &msg);

    void PrivateChat();

    ~Client();
};

#endif