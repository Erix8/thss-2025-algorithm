cmake_minimum_required(VERSION 3.10)
project(Sorting)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Each .cpp is compiled as an independent translation unit and linked together.
add_executable(sorting
    src/main.cpp
    src/Sort.cpp
    src/Test.cpp
)