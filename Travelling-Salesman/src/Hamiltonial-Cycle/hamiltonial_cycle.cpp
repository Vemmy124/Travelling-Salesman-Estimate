#include "hamiltonial_cycle.hpp"

geometry::Graph ExtractGraph(const geometry::Triangulation& triangulation, const std::vector<Vector2D>& points) {
    geometry::Graph result(points.size());
    for (const auto& [edge, _] : triangulation) {
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

double GetRouteLength(const std::vector<int>& route, const std::vector<Vector2D>& points) {
    double result = GetLonLatDistance(Edge(route.back(), route[0]), points);
    for (size_t i = 1; i < points.size(); ++i) {
        result += GetLonLatDistance(Edge(route[i - 1], route[i]), points);
    }
    return result;
}

std::vector<int> GetHamiltonialCycle(std::vector<Vector2D>& points) {
    auto triangulation_solver = geometry::DelaunayBuilder::Create(std::move(points));

    auto triangulation = triangulation_solver->Get();
    points = std::move(triangulation.points);

    auto graph = ExtractGraph(triangulation.graph, points);

    KruskalAlgorithm kruskal_solver(graph, points);
    graph = kruskal_solver.Solve();

    auto sequence = GetTravellingSalesmanSolution(graph);

    // Check that the cycle is hamiltonial
    assert(sequence.size() == points.size());
    assert(std::unordered_set<int>(sequence.begin(), sequence.end()).size() == sequence.size());

    return sequence;
}

