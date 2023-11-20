cmake_minimum_required(VERSION 3.12)
project(Final_task)

set(CMAKE_CXX_STANDARD 20)

include_directories(/usr/include)
link_directories(/usr/lib/x86_64-linux-gnu)

add_subdirectory(Server)
add_subdirectory(Client)
