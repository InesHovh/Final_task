// #include "../headers/Server.hpp"
#include "../../includes.hpp"

int main() {
    Server server("1245");
    Database database("mydb", "ines", "pass", "localhost", "5432");

    database.ConnectionToServer();
    return 0;
}