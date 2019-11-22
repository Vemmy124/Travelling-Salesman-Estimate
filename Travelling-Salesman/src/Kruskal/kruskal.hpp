#pragma once

#include "../Delaunay-Triangulation/triangulation.hpp"
#include "../models/models.hpp"

dataT GetLonLatDistance(const Edge& edge, const std::vector<CVec2>& points);

class KruskalAlgorithm {
 public:
  KruskalAlgorithm(models::Graph graph, std::vector<CVec2> points) :
      graph_(std::move(graph)), points_(std::move(points)),
      parent_(points_.size()) {
    for (int i = 0; i < parent_.size(); ++i) {
      parent_[i] = i;
    }
  }
  models::Graph Solve();
 private:
  std::vector<Edge> get_edge_graph() const;
  int find_root_update(int v);

  models::Graph graph_;
  std::vector<CVec2> points_;
  std::vector<int> parent_;
};
