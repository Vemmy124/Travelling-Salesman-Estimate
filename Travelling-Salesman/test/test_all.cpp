#include "gtest/gtest.h"

#define private public  // never do this
#include "../src/Hamiltonial-Cycle/hamiltonial_cycle.hpp"

#include <cstdlib>
#include <fstream>

void FileTestBody(std::ifstream&& in);

TEST(Triangulation, FileInput1) {
  std::ifstream in;
  in.open("../test/tests/001");
  FileTestBody(std::move(in));
}

TEST(Triangulation, FileInput2) {
  std::ifstream in;
  in.open("../test/tests/002");
  FileTestBody(std::move(in));
}

TEST(Triangulation, FileInput3) {
  std::ifstream in;
  in.open("../test/tests/003");
  FileTestBody(std::move(in));
}

auto GetPointsFromStream(std::ifstream& in) {
  std::vector<geometry::Vector2D> points;

  // Считывание пар точек до EOF
  double x, y;
  while ((in >> x) && (in >> y)) {
    points.emplace_back(x, y);
  }
  return points;
}

void Dfs(int v,
         double& dist,
         const std::vector<geometry::Vector2D>& points,
         std::vector<bool>& visited,
         std::vector<int>& path,
         std::vector<int>& result,
         double& best_dist) {
  visited[v] = true;
  path.emplace_back(v);
  if (path.size() == points.size()) {
    auto weight = GetLonLatDistance(Edge(v, 0), points);
    if (dist + weight < best_dist) {
      best_dist = dist + weight;
      result = path;
    }
  }
  for (int i = 0; i < points.size(); ++i) {
    if (!visited[i]) {
      auto weight = GetLonLatDistance(Edge(v, i), points);
      dist += weight;
      if (dist < best_dist) {
        Dfs(i, dist, points, visited, path, result, best_dist);
      }
      dist -= weight;
    }
  }
  path.pop_back();
  visited[v] = false;
}

std::vector<int> BruteHamiltonial(const std::vector<geometry::Vector2D>& points) {
  std::vector<int> path;
  std::vector<int> result;
  std::vector<bool> visited(points.size(), false);
  double dist = 0.0;
  double best_dist = 1e18;
  Dfs(0, dist, points, visited, path, result, best_dist);

  // Check that the cycle is hamiltonial
  assert(result.size() == points.size());
  assert(std::unordered_set<int>(result.begin(), result.end()).size()
             == result.size());

  return result;
}

void FileTestBody(std::ifstream&& in) {
  auto points = GetPointsFromStream(in);
  auto sequence = GetHamiltonialCycle(points);
  std::cout << std::setprecision(9)
            << GetRouteLength(sequence, points) << " found length" << std::endl;
  if (points.size() < 12) {
    auto best_sequence = BruteHamiltonial(points);
    std::cout << GetRouteLength(best_sequence, points) << " best length"
              << std::endl;
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

