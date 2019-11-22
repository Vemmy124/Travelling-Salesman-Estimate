#pragma once

#include <unordered_map>

namespace models {
using Triangulation = std::unordered_map<Edge, TwoVertices, EdgeHasher>;
using Graph = std::vector<std::vector<int>>;
}
