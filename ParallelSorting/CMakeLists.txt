cmake_minimum_required(VERSION 3.10)
project(ParallelSorting)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(parallel_sorting
    src/main.cpp
    src/ParallelSort.cpp
)

# AppleClang requires Homebrew libomp for OpenMP support
if(APPLE)
    target_compile_options(parallel_sorting PRIVATE
        -Xclang -fopenmp
        -I/opt/homebrew/opt/libomp/include
    )
    target_link_libraries(parallel_sorting PRIVATE
        -L/opt/homebrew/opt/libomp/lib
        -lomp
    )
else()
    find_package(OpenMP REQUIRED)
    target_link_libraries(parallel_sorting PRIVATE OpenMP::OpenMP_CXX)
endif()