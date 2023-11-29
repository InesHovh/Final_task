cmake_minimum_required(VERSION 3.12)
project(Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Release)

# install conan libs
execute_process(COMMAND conan install ${PROJECT_SOURCE_DIR}/conanfile.txt --output-folder=${PROJECT_BINARY_DIR} --build=missing)
include(${PROJECT_BINARY_DIR}/conan_toolchain.cmake)

# include 3rdParties libs
find_package(libpqxx REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/headers ${libpqxx_INCLUDE_DIR})

file(GLOB SRCS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE libpqxx::pqxx)

# include_directories(${CMAKE_SOURCE_DIR}/headers)





