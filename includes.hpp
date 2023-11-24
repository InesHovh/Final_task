#ifndef INCLUDES_HPP
# define INCLUDES_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>
#include <array>
#include <map>
#include <fcntl.h>
#include <set>
#include <stdint.h>

#include "Server/headers/Server.hpp"
#include "Client/headers/Client.hpp"
#include "Server/headers/Database.hpp"

struct User {
    uint16_t    start_byte;
    uint8_t     username_size;
    char        username[255];
    uint8_t     pass_size;
    char        pass[255];
    uint16_t    crc_checksum;
};

struct SendMsg {
    uint16_t    start_byte;
    uint8_t     username_size;
    char        username[255];
    uint16_t    msg_size;
    char        msg[500];
    uint16_t    crc_checksum;
};

struct GetMsgs {
    uint16_t    start_byte;
};

struct Updatemsg {
    uint16_t    start_byte;
    uint16_t    packet_counts;
    uint16_t    crc_checksum;
};

struct Packet {
    uint16_t    start_byte;
    uint32_t    timestamp;
    uint8_t     username_size;
    char        username[255];
    uint16_t    msg_size;
    char        msg[500];
    uint16_t    crc_checksum;
};

#endif