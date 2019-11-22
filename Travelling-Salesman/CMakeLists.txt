cmake_minimum_required(VERSION 3.12)
project(Travelling_Salesman)

set(CMAKE_CXX_STANDARD 17)

add_executable(Travelling_Salesman main.cpp src/Delaunay-Triangulation/triangulation.cpp src/Kruskal/kruskal.cpp)