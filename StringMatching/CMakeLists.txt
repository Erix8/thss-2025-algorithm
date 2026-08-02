cmake_minimum_required(VERSION 3.10)
project(StringMatching)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(string_matching
    src/main.cpp
    src/StringMatching.cpp
)