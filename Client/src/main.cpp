// main.cpp

#include "../headers/Client.hpp"

int main() {
    Client client("1234", "127.0.0.1");

    if (client.Start()) {
        std::cout << "Connected to the server.\n";

        std::cout << "Command info: " << std::endl;
        std::cout << "1. Sign Up " << std::endl;
        std::cout << "2. Sign In " << std::endl;
        std::cout << "3. Exit " << std::endl;
        std::cout << std::endl;


        while (1) {
            std::cout << "Enter the number of the command : ";
            std::string command;
            std::getline(std::cin, command);

            if (command == "1") {
                client.SendRegistrationRequest();
                std::cout << "Registration successful.\n";
                break;
            } else if (command == "2") {
                client.SendLoginRequest();
                std::cout << "Durs eka " << std::endl;
                break;
            } else if (command == "3") {
                exit(0);
            } else {
                std::cout << "Please type a correct command." << std::endl;
                continue;
            }
        }

        std::cout << "For private messaging, you must have the list of online users." << std::endl;
        std::cout << "Here is :" << std::endl;
        client.GetUsersList();
        std::cout << std::endl;

        std::string receiverUsername;
        std::cout << "Enter the username of the user you want to message (or type 'Exit' to quit): ";
        std::getline(std::cin, receiverUsername);

        while (1) {

            if (receiverUsername == "Exit") {
                break;
            }

            std::string message;
            std::cout << "Message to " << receiverUsername << "  ";
            std::getline(std::cin, message);

            if (message == "Exit") {
                break;
            }
            
            client.SendToSecondUser(receiverUsername, message);

            // std::string response;
            // if (client.ReceiveToTheFirst(response))
            //     std::cout << "Response from " << receiverUsername << std::endl;
            // else 
            //     std::cerr << "Failed to receive " << std::endl;
        }
    } else {
        std::cerr << "Failed to connect to the server.\n";
    }

    return 0;
}
