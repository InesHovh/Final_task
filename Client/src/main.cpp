#include "../headers/Client.hpp"

int main() {
    Client client("1245", "127.0.0.1");

    if (client.Start()) {
        std::cout << "Connected to the server. \n";

        std::cout << "Command info: " << std::endl;
        std::cout << "1. Sign Up " << std::endl;
        std::cout << "2. Sign In " << std::endl;
        std::cout << "3. Exit " << std::endl;
        std::cout << std::endl;

        while(1){
            std::cout << "Enter the number of the command : " ;
            std::string command;
            std::getline(std::cin, command);

            if (command == "1")
            {
                client.SendRegistrationRequest();
                std::cout << "Hasel em " << std::endl;
                break;
            } else if (command == "2") {
                client.SendLoginRequest();
                break;
            } else if (command == "Exit") {
                exit(0);
            }
            else
            {
                std::cout << "Please type correct command." << std::endl;
                continue;
            }
        }

        while(1){
            std::cout << "For chatting you must have list of the users " << std::endl;
            std::cout << "Here is the users: " << std::endl;
            client.GetUsersList();
            std::cout << std::endl;
            std::string user;
            std::cout << "Type the name of user " << std::endl;
            std::getline(std::cin, user);

                // std::cout << "Now choose option for chatting: " << std::endl;

            // std::cout << "1. PM " << std::endl;
            // std::cout << "2. Create group chat " << std::endl;
            // std::cout << std::endl;
            client.PrivateMsgs(user);
            // while(1) {
            //     std::getline(std::cin, com);

            //     if (com == "1"){
            //         client.PrivateMsgs();
            //     } else if (com == "2") {
            //         client.GroupChat();
            //     } else {
            //         std::cout << "Please type correct command." << std::endl;
            //         continue;
            //     }
            // }

            // std::string msg;
            // std::string receivedMsg;
            // std::cout << "Enter the message to send " << std::endl;
            // std::getline(std::cin, msg);
            
            // if (!client.SendMsgToServer(msg)) {
            //     std::cout << "Disconnecting " << std::endl;
            //     break;
            // }
            // if (!client.ReceiveMsgFromServer(receivedMsg, sizeof(receivedMsg))) {
            //     std::cout << "Disconnected from server" << std::endl;
            //     break;
            // }

            // std::cout << "Received from server: " << receivedMsg << std::endl;
        }
    } else {
        std::cerr << "Failed to connect to the server.\n";
    }

    return 0;
}
