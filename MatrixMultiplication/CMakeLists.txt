cmake_minimum_required(VERSION 3.10)
project(MatrixMultiplication)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(matrix_multiplication
    src/main.cpp
    src/MatrixMultiplication.cpp
)

# AppleClang requires Homebrew libomp for OpenMP support
if(APPLE)
    target_compile_options(matrix_multiplication PRIVATE
        -Xclang -fopenmp
        -I/opt/homebrew/opt/libomp/include
    )
    target_link_libraries(matrix_multiplication PRIVATE
        -L/opt/homebrew/opt/libomp/lib
        -lomp
    )
else()
    find_package(OpenMP REQUIRED)
    target_link_libraries(matrix_multiplication PRIVATE OpenMP::OpenMP_CXX)
endif()