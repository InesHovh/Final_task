cmake_minimum_required(VERSION 3.12)
project(Client)

set(CMAKE_CXX_STANDARD 20)

# find_path(TheHeaderFile util.hpp
#         PATHS /home/user/Desktop/Application/Utils)

# find_library(TheLibrary
#         NAMES Utils
#         PATHS /home/user/Desktop/Application/Utils/build)

# add_executable(Client main.cpp Client.cpp)

file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(Client PRIVATE)
# target_link_libraries(Client PRIVATE)