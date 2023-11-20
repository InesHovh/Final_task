cmake_minimum_required(VERSION 3.12)
project(Server)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/headers)

# add_subdirectory(../DatabaseLib)
# file(GLOB SERVER_SOURCES "${SMAKE_SOURCE_DIR}/src/*.cpp")

# target_link_directories(Server PRIVATE path/to/DatabaseLib/build/directory)

set(SERVER_SOURCES
    src/Server.cpp
    src/Database.cpp
    src/main.cpp)

add_executable(Server ${SERVER_SOURCES})

target_link_libraries(Server PRIVATE pqxx)

