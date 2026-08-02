cmake_minimum_required(VERSION 3.18)
project(SeamCarving)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- pybind11 (optional -- only needed for Python module) ----
find_package(pybind11 QUIET)

# ---- C++ shared sources (used by both CLI and Python module) ----
add_library(seamcarving_core STATIC
    src/SeamCarve.cpp
    src/util_image.cpp
)
target_include_directories(seamcarving_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
           ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---- CLI executable (always built) ----
add_executable(seam_carving
    src/main.cpp
)
target_link_libraries(seam_carving seamcarving_core)
target_include_directories(seam_carving PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ---- Python extension module (only if pybind11 is available) ----
if(pybind11_FOUND)
    pybind11_add_module(_seamcarving_core
        pybind/seamcarving_bind.cpp
    )
    target_link_libraries(_seamcarving_core PRIVATE seamcarving_core)
    target_include_directories(_seamcarving_core
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    # Put .so/.pyd directly into python/seamcarving/ for pip import
    set_target_properties(_seamcarving_core PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/python/seamcarving"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/python/seamcarving"
    )
endif()