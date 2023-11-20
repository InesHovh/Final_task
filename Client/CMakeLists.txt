cmake_minimum_required(VERSION 3.12)
project(Client)

set(CMAKE_CXX_STANDARD 20)

include_directories("${PROJECT_SOURCE_DIR}/headers")

file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp" )

set(SOURCES ${SRC_FILES})

add_executable(Client ${SOURCES})