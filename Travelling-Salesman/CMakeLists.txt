cmake_minimum_required(VERSION 3.10)
project(Travelling_Salesman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2")

add_executable(Travelling_Salesman
        main.cpp
        src/Delaunay-Triangulation/scanline_triangulation.cpp
        src/Kruskal/kruskal.cpp
        src/models/geometry.cpp
        )