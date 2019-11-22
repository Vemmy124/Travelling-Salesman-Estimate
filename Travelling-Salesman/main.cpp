#include <cmath>
#include <iostream>
#include <vector>

#include "src/Delaunay-Triangulation/triangulation.hpp"
#include "src/Kruskal/kruskal.hpp"
#include "src/models/models.hpp"

void OpenFile(const char* name) {
  freopen(name, "r", stdin);
}

std::vector<CVec2> ReadPointsFromStdin() {
  std::vector<CVec2> result;
  dataT x, y;
  while ((std::cin >> x) && (std::cin >> y)) {
    result.emplace_back(x, y);
  }
  return result;
}

models::Graph ExtractGraph(const models::Triangulation& triangulation, const std::vector<CVec2>& points) {
  models::Graph result(points.size());
  for (auto& [edge, _] : triangulation) {
    result[edge.v1].emplace_back(edge.v2);
    result[edge.v2].emplace_back(edge.v1);
  }
  return result;
}

void DFS(int v, int last_visited, const models::Graph& graph, std::vector<int>& result) {
  result.emplace_back(v);
  for (const auto adjacent : graph[v]) {
    if (adjacent != last_visited) {
      DFS(adjacent, v, graph, result);
    }
  }
}

std::vector<int> GetTravellingSalesmanSolution(const models::Graph& graph) {
  std::vector<int> result;
  DFS(0, -1, graph, result);
  return result;
}

dataT GetDistance(const CVec2& fst, const CVec2& snd) {
  dataT dx = fst.x - snd.x;
  dataT dy = fst.y - snd.y;
  return sqrt(dx * dx + dy * dy);
}

dataT GetRouteLength(const std::vector<int>& route, const std::vector<CVec2>& points) {
  dataT result = GetLonLatDistance(Edge(route.back(), 0), points);
  for (size_t i = 1; i < points.size(); ++i) {
    result += GetLonLatDistance(Edge(i - 1, i), points);
  }
  return result;
}

int main() {
  OpenFile("input.txt");

  auto points = ReadPointsFromStdin();
  TriangulationAlgorithm triangulation_solver(points);
  triangulation_solver.Solve();

  models::Triangulation triangulation;
  triangulation_solver.GetResult(triangulation, points);

  auto graph = ExtractGraph(triangulation, points);

  KruskalAlgorithm kruskal_solver(graph, points);
  graph = kruskal_solver.Solve();

  auto sequence = GetTravellingSalesmanSolution(graph);

  std::cout << GetRouteLength(sequence, points) << std::endl;

  return 0;
}