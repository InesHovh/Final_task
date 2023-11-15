cmake_minimum_required(VERSION 3.12)
project(Client)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES Client.cpp)

add_executable(Client ${SOURCES} Client.hpp)