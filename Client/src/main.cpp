#include "../headers/Client.hpp"

// int main() {
//     Client client("1245", "127.0.0.1");
//     if (client.Start()) {
//         std::cout << "Connected successfully\n" <<  std::endl;

        // std::cout << "Command info: " << std::endl;
        // std::cout << "1. SignUp " << std::endl;
        // std::cout << "2. SignIn " << std::endl;
        // std::cout << "3. EXIT " << std::endl;
        // std::cout << std::endl;
        // std::cout << "Enter the command or the number: " << std::endl;
        // while (true) {

        //     std::string command;
        //     std::getline(std::cin, command);

        //     if (command == "SignUp" || command == "1"){
        //         client.SignUp();
        //     } else if (command == "SignIn" || command == "2"){
        //         client.SignIn();
        //     } else if (command == "EXIT" || command == "3" || command == ""){
        //         exit(0);
        //     }

        //     std::string msg;
        //     std::string receivedMsg;
        //     std::cout << "Enter the message to send " << std::endl;
        //     std::getline(std::cin, msg);
            
        //     if (!client.SendMsgToServer()) {
        //         std::cout << "Disconnecting " << std::endl;
        //         break;
        //     }
        //     if (!client.ReceiveMsgFromServer(receivedMsg)) {
        //         std::cout << "Disconnected from server" << std::endl;
        //         break;
        //     }

        //     std::cout << "Received from server: " << receivedMsg << std::endl;
        // }
        
    // }

    // return 0;
// }


int main() {
    Client client("1234", "127.0.0.1");

    if (client.Start()) {
        std::cout << "Connected to the server. \n";

        std::cout << "Sending login request...\n";
        client.AddUser();
        if (client.SendLoginRequest()) {
            Response response;
            if (client.ReceiveResponse(response)) {
                if (response.OK == 0x01) {
                    std::cout << "Login successful.\n";
                } else {
                    std::cout << "Login failed.\n";
                }
            } else {
                std::cerr << "Failed to receive response from server.\n";
            }
        } else {
            std::cerr << "Failed to send login request to server.\n";
        }

        std::cout << "Sending registration request...\n";
        client.AddUser();
        if (client.SendRegistrationRequest()) {
            Response response;
            if (client.ReceiveResponse(response)) {
                if (response.OK == 0x01) {
                    std::cout << "Registration successful.\n";
                } else {
                    std::cout << "Registration failed.\n";
                }
            } else {
                std::cerr << "Failed to receive response from server.\n";
            }
        } else {
            std::cerr << "Failed to send registration request to server.\n";
        }
    } else {
        std::cerr << "Failed to connect to the server.\n";
    }

    return 0;
}
