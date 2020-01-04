#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "src/Hamiltonial-Cycle/hamiltonial_cycle.hpp"

using geometry::Vector2D;

void OpenFile(const std::string& name) {
  freopen(name.c_str(), "r", stdin);
}

std::vector<Vector2D> ReadPointsFromStdin() {
  std::vector<Vector2D> result;
  double x, y;
  while ((std::cin >> x) && (std::cin >> y)) {
    result.emplace_back(x, y);
  }
  return result;
}

int main() {
  OpenFile("input.txt");

  auto points = ReadPointsFromStdin();
  auto sequence = GetHamiltonialCycle(points);
  std::cout << std::setprecision(9) << GetRouteLength(sequence, points)
            << "km is the estimated hamiltonial path length" << std::endl;

  return 0;
}