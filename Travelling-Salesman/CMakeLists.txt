cmake_minimum_required(VERSION 3.10)
project(Travelling_Salesman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2")

add_executable(Travelling_Salesman
        main.cpp
        src/Delaunay-Triangulation/scanline_triangulation.cpp
        src/Hamiltonial-Cycle/hamiltonial_cycle.cpp
        src/Kruskal/kruskal.cpp
        src/models/geometry.cpp
        )

# Uncomment for gtest

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(Tests
        src/Delaunay-Triangulation/scanline_triangulation.cpp
        src/Hamiltonial-Cycle/hamiltonial_cycle.cpp
        src/Kruskal/kruskal.cpp
        src/models/geometry.cpp
        test/test_all.cpp
        )
target_link_libraries(Tests ${GTEST_LIBRARIES} pthread)