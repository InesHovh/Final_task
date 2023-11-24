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
        std::cout << "Enter the command or the number : " ;

        while(1){
            std::string command;
            std::getline(std::cin, command);

            if (command == "Sign Up" || command == "1")
            {
                client.SendRegistrationRequest();
            } else if (command == "Sign In" || command == "2") {
                client.SendLoginRequest();    
            } else if (command == "Exit" || command == "") {
                exit(0);
            }
            else
            {
                std::cout << "Please type correect command" << std::endl;
                continue;
            }

            // std::string msg;
            // std::string receivedMsg;
            // std::cout << "Enter the message to send " << std::endl;
            // std::getline(std::cin, msg);
            
            // if (!client.SendMsgToServer(msg, sizeof(msg))) {
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
