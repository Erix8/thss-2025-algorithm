cmake_minimum_required(VERSION 3.10)
project(ClosestPoints)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Each .cpp is compiled as an independent translation unit and linked together.
add_executable(closest_points
    src/main.cpp
    src/Points.cpp
    src/FindNearestPoints.cpp
    src/FileIO.cpp
    src/Test.cpp
)
