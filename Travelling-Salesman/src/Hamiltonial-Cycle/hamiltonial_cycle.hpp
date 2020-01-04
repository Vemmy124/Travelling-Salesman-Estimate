#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../Delaunay-Triangulation/scanline_triangulation.hpp"
#include "../Kruskal/kruskal.hpp"
#include "../models/geometry.hpp"

using geometry::Vector2D;

double GetRouteLength(const std::vector<int>& route,
                      const std::vector<Vector2D>& points);
std::vector<int> GetHamiltonialCycle(std::vector<Vector2D>& points);