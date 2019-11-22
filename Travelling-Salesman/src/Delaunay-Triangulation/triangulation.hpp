#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <unordered_map>
#include <vector>

// Это тип координат точек. Его можно поменять на целочисленный тип, убрав вычисления с погрешностями
typedef double dataT;

// Вектор в двумерном пространстве
struct CVec2 {
  CVec2() = default;
  CVec2(dataT _x, dataT _y, int _left = 0, int _right = 0) :
      x(_x), y(_y), left(_left), right(_right) {}
  dataT x, y;
  int left,
      right;    // Эти поля отвечают за соседей слева и справа в МВО, можно их убрать и хранить двусвязный список отдельно

  CVec2 operator+(const CVec2 &other) const;
  CVec2 operator-(const CVec2 &other) const;
  CVec2 operator-() const;
};

// Тип ребро, фактически работает как std::pair<int, int>
struct Edge {
  Edge(int _v1 = -1, int _v2 = -1) :
      v1(_v1), v2(_v2) {}
  int v1;    // Вершины, которые соединяет ребро
  int v2;
  bool operator==(const Edge &other) const {
    return v1 == other.v1 && v2 == other.v2;
  }
};

// Определение хэш-функции для ребра, как комбинация хэшей вершин
struct EdgeHasher {
  std::hash<int> hasher;
  size_t operator()(const Edge& edge) const {
    size_t lhs = hasher(edge.v1);
    size_t rhs = hasher(edge.v2);
    lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    return lhs;
  }
};

// Множество из 2 вершин, нужна для удобства
struct TwoVertices {
  explicit TwoVertices(int v1 = -1, int v2 = -1) : outer1(v1), outer2(v2) {}
  int outer1;
  int outer2;
  void insert(int v) {
    if (outer1 == v || outer2 == v) return;
    assert(this->size() < 2);
    (outer1 == -1 ? outer1 : outer2) = v;
  }
  void erase(int v) {
    if (outer1 == v) {
      outer1 = -1;
    } else if (outer2 == v) {
      outer2 = -1;
    }
  }
  int GetMax() {
    assert(outer1 != -1 || outer2 != -1);
    return std::max(outer1, outer2);
  }
  int GetMin() {
    if (outer1 != -1 && outer2 != -1) {
      return std::min(outer1, outer2);
    }
    assert(outer1 != -1 || outer2 != -1);
    return outer1 != -1 ? outer1 : outer2;
  }
  int size() { return (outer1 != -1) + (outer2 != -1); }
};

class TriangulationAlgorithm {

 public:
  TriangulationAlgorithm(std::vector<CVec2> points) : points_(std::move(points)) {}
  void Solve();
  void GetResult(std::unordered_map<Edge, TwoVertices, EdgeHasher>& triangulation,
                 std::vector<CVec2>& points);
 private:
  // Функция, которая рекурсивно перестраивает триангуляцию (шаг 4)
  void restructure(int left, int right, int cur);

  // Проверяет, нужно ли перестраивать четырехугольник, заданный индексами точек в аргументах
  bool check(int left, int right, int cur, int innerPt);

  std::vector<CVec2> points_;    // Храним точки как радиус-вектора
  std::vector<Edge> recursion_stack_;    // Моделируем рекурсию с помощью стека, чтобы не было stack overflow
  std::unordered_map<Edge, TwoVertices, EdgeHasher>
      triangulation_;  // Структура для хранения триангуляции: ребро -> пара вершин
};
