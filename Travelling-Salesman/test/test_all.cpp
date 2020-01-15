#include "gtest/gtest.h"

#define private public  // never do this
#include "../src/Hamiltonial-Cycle/hamiltonial_cycle.hpp"

#include <cstdlib>
#include <fstream>

void FileTestBody(std::ifstream&& in);

TEST(UniVariate, EightPoints) {
  std::ifstream in;
  in.open("../test/tests/uniform/001");
  FileTestBody(std::move(in));
}

TEST(UniVariate, NinePoints) {
  std::ifstream in;
  in.open("../test/tests/uniform/002");
  FileTestBody(std::move(in));
}

TEST(UniVariate, TenPoints) {
  std::ifstream in;
  in.open("../test/tests/uniform/003");
  FileTestBody(std::move(in));
}

TEST(UniVariate, BigInput) {
  std::ifstream in;
  in.open("../test/tests/uniform/004");
  FileTestBody(std::move(in));
}

TEST(NormVariate, EightPoints) {
  std::ifstream in;
  in.open("../test/tests/normal/001");
  FileTestBody(std::move(in));
}

TEST(NormVariate, NinePoints) {
  std::ifstream in;
  in.open("../test/tests/normal/002");
  FileTestBody(std::move(in));
}

TEST(NormVariate, TenPoints) {
  std::ifstream in;
  in.open("../test/tests/normal/003");
  FileTestBody(std::move(in));
}

TEST(NormVariate, BigInput) {
  std::ifstream in;
  in.open("../test/tests/normal/004");
  FileTestBody(std::move(in));
}

TEST(RealData, RussianCities) {
  std::ifstream in;
  in.open("../test/tests/real/001");
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
  EXPECT_EQ(result.size(), points.size());
  EXPECT_EQ(std::unordered_set<int>(result.begin(), result.end()).size(),
      result.size());

  return result;
}

void FileTestBody(std::ifstream&& in) {
  auto points = GetPointsFromStream(in);
  auto sequence = GetHamiltonialCycle(points);

  auto found_length = GetRouteLength(sequence, points);
  std::cout << std::setprecision(9)
            << found_length << " found length" << std::endl;
  if (points.size() < 12) {
    auto best_sequence = BruteHamiltonial(points);
    auto best_length = GetRouteLength(best_sequence, points);
    std::cout << best_length <<
    " best length\n"
    "Precision " << found_length / best_length << std::endl;
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

