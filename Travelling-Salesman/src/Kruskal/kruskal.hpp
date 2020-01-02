#pragma once

#include "../Delaunay-Triangulation/scanline_triangulation.hpp"
#include "../models/geometry.hpp"

using geometry::Edge;
using geometry::Graph;
using geometry::Vector2D;

double GetLonLatDistance(const Edge& edge, const std::vector<Vector2D>& points);

class KruskalAlgorithm {
 public:
  KruskalAlgorithm(Graph graph, std::vector<Vector2D> points) :
      graph_(std::move(graph)), points_(std::move(points)),
      parent_(points_.size()) {
    for (int i = 0; i < parent_.size(); ++i) {
      parent_[i] = i;
    }
  }
  Graph Solve();
 private:
  std::vector<Edge> get_edge_graph() const;
  int find_update_root(int v);

  Graph graph_;
  std::vector<Vector2D> points_;
  std::vector<int> parent_;
};
