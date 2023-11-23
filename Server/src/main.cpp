#include "../headers/Server.hpp"


int main() {
    Server server("1234");
    Database database("mydb", "ines", "pass", "localhost", "5432");

    server.ConnectionToDB(database);
    return 0;
}