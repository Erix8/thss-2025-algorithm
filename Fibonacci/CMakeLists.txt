cmake_minimum_required(VERSION 3.10)
project(Fibonacci)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(fibonacci
    src/main.cpp
    src/Fibonacci.cpp
)