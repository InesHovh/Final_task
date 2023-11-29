#include "../headers/Server.hpp"

int main() {
    Database database("mydb", "ines", "pass", "127.0.0.1", "5432");
    Server server("1234", &database);

    server.ConnectionToDB(database);
    return 0;
}