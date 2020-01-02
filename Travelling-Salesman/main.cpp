#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "src/Delaunay-Triangulation/scanline_triangulation.hpp"
#include "src/Kruskal/kruskal.hpp"
#include "src/models/geometry.hpp"

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

geometry::Graph ExtractGraph(const geometry::Triangulation& triangulation, const std::vector<Vector2D>& points) {
  geometry::Graph result(points.size());
  for (auto& [edge, _] : triangulation) {
    result[edge.v1].emplace_back(edge.v2);
    result[edge.v2].emplace_back(edge.v1);
  }
  return result;
}

void DFS(int v, int last_visited, const geometry::Graph& graph, std::vector<int>& result) {
  result.emplace_back(v);
  for (const auto adjacent : graph[v]) {
    if (adjacent != last_visited) {
      DFS(adjacent, v, graph, result);
    }
  }
}

std::vector<int> GetTravellingSalesmanSolution(const geometry::Graph& graph) {
  std::vector<int> result;
  DFS(0, -1, graph, result);
  return result;
}

double GetDistance(const geometry::Vector2D& fst, const geometry::Vector2D& snd) {
  double dx = fst.x - snd.x;
  double dy = fst.y - snd.y;
  return sqrt(dx * dx + dy * dy);
}

double GetRouteLength(const std::vector<int>& route, const std::vector<geometry::Vector2D>& points) {
  double result = GetLonLatDistance(Edge(route.back(), route[0]), points);
  for (size_t i = 1; i < points.size(); ++i) {
    result += GetLonLatDistance(Edge(route[i - 1], route[i]), points);
  }
  return result;
}

int main() {
  OpenFile("input.txt");

  auto points = ReadPointsFromStdin();
  auto triangulation_solver = geometry::DelaunayBuilder::Create(std::move(points));

  auto triangulation = triangulation_solver->Get();
  points = triangulation.points;

  auto graph = ExtractGraph(triangulation.graph, points);

  KruskalAlgorithm kruskal_solver(graph, points);
  graph = kruskal_solver.Solve();

  auto sequence = GetTravellingSalesmanSolution(graph);

  std::cout << std::fixed << std::setprecision(1) << GetRouteLength(sequence, points)
    << " is the estimated hamiltonial path length" << std::endl;

  // Check that the cycle is hamiltonial
  assert(sequence.size() == points.size());
  assert(std::unordered_set<int>(sequence.begin(), sequence.end()).size() == sequence.size());

  return 0;
}