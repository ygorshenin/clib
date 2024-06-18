#pragma once

#include "geom/point.h"

#include <algorithm>
#include <vector>

namespace algo::geom {
template <typename T>
struct IsCW {
  static bool IsGood(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& c) {
    const auto ba = b - a;
    const auto cb = c - b;
    return Cross(ba, cb) < 0;
  }
};

template <typename T>
struct IsCCW {
  static bool IsGood(const Point2D<T>& a, const Point2D<T>& b, const Point2D<T>& c) {
    const auto ba = b - a;
    const auto cb = c - b;
    return Cross(ba, cb) > 0;
  }
};

template <typename T, typename Strategy>
void AddToHull(std::vector<Point2D<T>>& vs, const Point2D<T>& p) {
  while (vs.size() >= 2 && !Strategy::IsGood(vs[vs.size() - 2], vs[vs.size() - 1], p))
    vs.pop_back();
  vs.push_back(p);
}

template <typename T>
std::vector<Point2D<T>> ConvexHull(std::vector<Point2D<T>> points) {
  using Point = Point2D<T>;

  std::vector<Point> upperPart;
  std::vector<Point> lowerPart;

  std::sort(points.begin(), points.end(), [](const Point& lhs, const Point& rhs) {
    if (lhs.m_x != rhs.m_x)
      return lhs.m_x < rhs.m_x;
    return lhs.m_y < rhs.m_y;
  });

  size_t i = 0;
  while (i < points.size()) {
    size_t j = i + 1;
    while (j < points.size() && points[i].m_x == points[j].m_x)
      ++j;

    AddToHull<T, IsCCW<T>>(lowerPart, points[i]);
    AddToHull<T, IsCW<T>>(upperPart, points[j - 1]);
    i = j;
  }

  if (!lowerPart.empty() && !upperPart.empty() && upperPart.front() == lowerPart.front())
    upperPart.erase(upperPart.begin());
  if (!lowerPart.empty() && !upperPart.empty() && upperPart.back() == lowerPart.back())
    upperPart.pop_back();

  lowerPart.insert(lowerPart.end(), upperPart.rbegin(), upperPart.rend());
  return lowerPart;
}
}  // namespace algo::geom
