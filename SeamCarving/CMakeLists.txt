cmake_minimum_required(VERSION 3.10)
project(SeamCarving)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(seam_carving
    src/main.cpp
    src/SeamCarve.cpp
    src/util_image.cpp
)

# stb_image.h and stb_image_write.h are header-only (their implementation
# is compiled via #define STB_IMAGE_IMPLEMENTATION in util_image.cpp).
target_include_directories(seam_carving PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})