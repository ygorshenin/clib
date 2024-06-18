#pragma once

#include <utility>

namespace algo::geom {
template <typename T>
struct Point2D {
  Point2D() = default;
  Point2D(T x, T y) : m_x{std::move(x)}, m_y{std::move(y)} {}

  bool operator==(const Point2D& rhs) const { return m_x == rhs.m_x && m_y == rhs.m_y; }

  Point2D& operator+=(const Point2D& rhs) {
    m_x += rhs.m_x;
    m_y += rhs.m_y;
    return *this;
  }

  Point2D& operator-=(const Point2D& rhs) {
    m_x -= rhs.m_x;
    m_y -= rhs.m_y;
    return *this;
  }

  Point2D operator+(const Point2D& rhs) const {
    auto result = *this;
    result += rhs;
    return result;
  }

  Point2D operator-(const Point2D& rhs) const {
    auto result = *this;
    result -= rhs;
    return result;
  }

  T m_x{};
  T m_y{};
};

template <typename T>
T Dot(const Point2D<T>& lhs, const Point2D<T>& rhs) {
  return lhs.m_x * rhs.m_x + lhs.m_y * rhs.m_y;
}

template <typename T>
T Cross(const Point2D<T>& lhs, const Point2D<T>& rhs) {
  return lhs.m_x * rhs.m_y - lhs.m_y * rhs.m_x;
}
}  // namespace algo::geom
