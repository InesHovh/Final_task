# Server/CMakeLists.txt

cmake_minimum_required(VERSION 3.12)
project(Server)

set(CMAKE_CXX_STANDARD 20)

# Include directories for finding headers
include_directories(${CMAKE_SOURCE_DIR}/Server)

# List your source files
set(SERVER_SOURCES
    Server.cpp
    Server.hpp
    # ${CMAKE_SOURCE_DIR}/Database/Database.cpp
    # ${CMAKE_SOURCE_DIR}/Database/Database.hpp
)

add_executable(Server ${SERVER_SOURCES})
target_link_libraries(Server PRIVATE DatabaseLib pqxx)

