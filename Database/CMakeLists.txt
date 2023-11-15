cmake_minimum_required(VERSION 3.12)
project(Database)

set(CMAKE_CXX_STANDARD 20)

# List your source files for the Database module
set(DATABASE_SOURCES
    Database.cpp
    Database.hpp
)

# Create a library or object file for the Database module
add_library(DatabaseLib OBJECT ${DATABASE_SOURCES})

# Database/CMakeLists.txt