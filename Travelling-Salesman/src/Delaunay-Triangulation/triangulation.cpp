#include "triangulation.hpp"

#include <algorithm>
#include <unordered_map>
#include <vector>

// Данной погрешности вычислений обычно хватает, но в теории могут возникать проблемы
const double eps = 1e-9;

CVec2 CVec2::operator+(const CVec2& other) const {
  return CVec2(x + other.x, y + other.y);
}

CVec2 CVec2::operator-(const CVec2& other) const {
  return CVec2(x - other.x, y - other.y);
}

CVec2 CVec2::operator-() const {
  return CVec2(-x, -y);
}

// Модуль векторного произведения со знаком
dataT CrossProduct(const CVec2& fst, const CVec2& snd) {
  return fst.x * snd.y - fst.y * snd.x;
}

// Компаратор для сортировки (в данном случае по координате х)
bool CompareByX(const CVec2& fst, const CVec2& snd) {
  return fst.x < snd.x;
}

void TriangulationAlgorithm::Solve() {
  // Размер стека рекурсии никогда не превышает количества точек
  recursion_stack_.resize(points_.size());
  // Сортировка по направлению
  std::sort(points_.begin(), points_.end(), CompareByX);

  if (points_.size() < 3) {
    // Если менее трех точек, то триангуляция пустая
    return;
  }

  // Инициализация первого треугольника
  if (CrossProduct(points_[1] - points_[0], points_[2] - points_[0]) < 0) {
    points_[0].left = 2;
    points_[0].right = 1;
    points_[1].left = 0;
    points_[1].right = 2;
    points_[2].left = 1;
    points_[2].right = 0;
  } else {
    points_[0].left = 1;
    points_[0].right = 2;
    points_[1].left = 2;
    points_[1].right = 0;
    points_[2].left = 0;
    points_[2].right = 1;
  }
  triangulation_[{1, 2}].insert(0);
  triangulation_[{0, 1}].insert(2);
  triangulation_[{0, 2}].insert(1);

  for (int i = 3; i < points_.size(); ++i) {
    int currentPt = i - 1;    // Последняя добавленная точка
    // Пока, двигаясь вправо от точки, ребра видимые, выполняем рекурсивное перестроение
    while (CrossProduct(points_[currentPt] - points_[i], points_[points_[currentPt].right] - points_[i]) > -eps) {
      restructure(currentPt, points_[currentPt].right, i);
      currentPt = points_[currentPt].right;
    }
    points_[i].right = currentPt;    // Добавляем правого соседа МВО текущей точки

    // Аналогично для движения влево, пока ребра видимые
    currentPt = i - 1;

    while (CrossProduct(points_[currentPt] - points_[i], points_[points_[currentPt].left] - points_[i]) < eps) {
      restructure(points_[currentPt].left, currentPt, i);
      currentPt = points_[currentPt].left;
    }
    points_[i].left = currentPt;    // Левый сосед МВО текущей точки

    points_[points_[i].right].left = i;    // Обновление соседей
    points_[currentPt].right = i;
  }

  // Тут в triangulation находится вся триангуляция в формате ребро -> две вершины для всех ребер, теперь можно с ней делать что угодно
}

void TriangulationAlgorithm::GetResult(
    std::unordered_map<Edge, TwoVertices, EdgeHasher>& triangulation, std::vector<CVec2>& points) {
  triangulation = triangulation_;
  points = points_;
}

// cur - всегда добавляемая точка, left и right - смежные с ней в треугольнике
// Четвертую точку для четырехугольника innerPt узнаем из ребра {left, right}
void TriangulationAlgorithm::restructure(int left, int right, int cur) {
  // Инициализация стека рекурсии
  // хранить достаточно left и right
  recursion_stack_[0] = {left, right};
  int stackSize = 1;
  while (stackSize > 0) {
    left = recursion_stack_[stackSize - 1].v1;
    right = recursion_stack_[stackSize - 1].v2;
    --stackSize;

    // Берем минимум в множестве, потому что cur > индекс любой уже добавленной точки
    int innerPt = triangulation_[{std::min(left, right), std::max(left, right)}].GetMin();
    if (check(left, right, cur, innerPt)) {
      // Если менять ничего в четырехугольнике не надо, просто добавляем недостающие ребра и выходим
      triangulation_[{right, cur}].insert(left);
      triangulation_[{left, cur}].insert(right);
      triangulation_[{std::min(left, right), std::max(left, right)}].insert(cur);
      continue;
    }

    // Иначе перестраиваем триангуляцию в четырехугольнике
    triangulation_[{right, cur}].erase(left);
    triangulation_[{right, cur}].insert(innerPt);
    triangulation_[{left, cur}].erase(right);
    triangulation_[{left, cur}].insert(innerPt);
    triangulation_[{std::min(innerPt, left), std::max(innerPt, left)}].erase(right);
    triangulation_[{std::min(innerPt, left), std::max(innerPt, left)}].insert(cur);
    triangulation_[{std::min(innerPt, right), std::max(innerPt, right)}].erase(left);
    triangulation_[{std::min(innerPt, right), std::max(innerPt, right)}].insert(cur);
    triangulation_.erase({std::min(left, right), std::max(left, right)});

    // И добавляем 2 новых рекурсивных вызова
    recursion_stack_[stackSize++] = {left, innerPt};
    recursion_stack_[stackSize++] = {innerPt, right};
  }
}

bool TriangulationAlgorithm::check(int left, int right, int cur, int innerPt) {
  // Проверка на то, что это вообще четырехугольник и выпуклость
  if (innerPt == cur || CrossProduct(points_[left] - points_[cur], points_[innerPt] - points_[cur]) < 0 ||
      CrossProduct(points_[right] - points_[cur], points_[innerPt] - points_[cur]) > 0) {
    return true;
  }
  // Проверка условия Делоне, как в книжке из статьи
  dataT Sa = (points_[cur].x - points_[right].x) * (points_[cur].x - points_[left].x) +
      (points_[cur].y - points_[right].y) * (points_[cur].y - points_[left].y);
  dataT Sb = (points_[innerPt].x - points_[right].x) * (points_[innerPt].x - points_[left].x) +
      (points_[innerPt].y - points_[right].y) * (points_[innerPt].y - points_[left].y);
  if (Sa > -eps && Sb > -eps) {
    return true;
  } else if (!(Sa < 0 && Sb < 0)) {
    dataT Sc = (points_[cur].x - points_[right].x) * (points_[cur].y - points_[left].y) -
        (points_[cur].y - points_[right].y) * (points_[cur].x - points_[left].x);
    dataT Sd = (points_[innerPt].x - points_[right].x) * (points_[innerPt].y - points_[left].y) -
        (points_[innerPt].y - points_[right].y) * (points_[innerPt].x - points_[left].x);
    if (Sc < 0) Sc = -Sc;
    if (Sd < 0) Sd = -Sd;
    if (Sc * Sb + Sa * Sd > -eps) {
      return true;
    }
  }
  return false;
}