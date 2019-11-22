#include "kruskal.hpp"

#include <cmath>

namespace {
dataT DegToRadians(const dataT& phi) {
  return phi * M_PI / 180.0;
}
}

const double R = 6378.1;

dataT GetLonLatDistance(const Edge& edge, const std::vector<CVec2>& points) {
  dataT x1 = DegToRadians(points[edge.v1].x);
  dataT x2 = DegToRadians(points[edge.v2].x);
  dataT dx = DegToRadians(points[edge.v1].x - points[edge.v2].x);
  dataT dy = DegToRadians(points[edge.v1].y - points[edge.v2].y);
  dataT a = sin(dx / 2) * sin(dx / 2) +
      cos(x1) * cos(x2) * sin(dy / 2) * sin(dy / 2);
  return R * 2 * atan2(sqrt(a), sqrt(1 - a));
}

dataT GetEdgeWeight(const Edge& edge, const std::vector<CVec2>& points) {
  dataT dx = points[edge.v1].x - points[edge.v2].x;
  dataT dy = points[edge.v1].y - points[edge.v2].y;
  return sqrt(dx * dx + dy * dy);
}

std::vector<Edge> KruskalAlgorithm::get_edge_graph() const {
  std::vector<Edge> result;
  for (size_t current = 0; current < graph_.size(); ++current) {
    for (const auto adjacent : graph_[current]) {
      if (current < adjacent) {
        result.emplace_back(current, adjacent);
      }
    }
  }
  return result;
}

int KruskalAlgorithm::find_root_update(int v) {
  int root;
  if (parent_[v] != v) {
    root = find_root_update(parent_[v]);
    return parent_[v] = root;
  }
  return v;
}

models::Graph KruskalAlgorithm::Solve() {
  auto edge_graph = get_edge_graph();
  sort(edge_graph.begin(), edge_graph.end(), [this](const Edge& fst, const Edge& snd) -> bool {
    return GetLonLatDistance(fst, points_) < GetLonLatDistance(snd, points_);
  });

  std::vector<Edge> minimal_span_tree;
  for (const auto& edge : edge_graph) {
    int v1_root = find_root_update(edge.v1);
    int v2_root = find_root_update(edge.v2);
    if (v1_root == v2_root) {
      continue;
    }
    parent_[v1_root] = v2_root;
    minimal_span_tree.emplace_back(edge);
  }

  models::Graph result(graph_.size());
  for (const auto& edge : minimal_span_tree) {
    result[edge.v1].emplace_back(edge.v2);
    result[edge.v2].emplace_back(edge.v1);
  }
  return result;
}
