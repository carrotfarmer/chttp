cmake_minimum_required(VERSION 3.26)
project(chttp)

set(CMAKE_CXX_STANDARD 20)

add_executable(chttp main.cpp
        http_server.hpp)
