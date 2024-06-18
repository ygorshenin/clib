#include <gtest/gtest.h>

#include "geom/hull.h"
#include "geom/point.h"

#include <vector>
using namespace std;

namespace algo::geom {
TEST(Hull, Smoke) {
  {
    vector<Point2D<int>> vs;
    const auto hull = ConvexHull(vs);
    ASSERT_TRUE(hull.empty());
  }
  {
    vector<Point2D<int>> vs{{1, 1}};
    const auto hull = ConvexHull(vs);
    ASSERT_EQ(hull.size(), 1);
    ASSERT_EQ(hull[0], Point2D<int>(1, 1));
  }
  {
    vector<Point2D<int>> vs{{1, 1}, {1, 1}};
    const auto hull = ConvexHull(vs);
    ASSERT_EQ(hull.size(), 1);
    ASSERT_EQ(hull[0], Point2D<int>(1, 1));
  }
  {
    vector<Point2D<int>> vs{{1, 1}, {3, 1}, {2, 1}};
    const auto hull = ConvexHull(vs);
    ASSERT_EQ(hull.size(), 2);
    ASSERT_EQ(hull[0], Point2D<int>(1, 1));
    ASSERT_EQ(hull[1], Point2D<int>(3, 1));
  }
  {
    vector<Point2D<int>> vs{{1, 1}, {3, 1}, {2, 2}};
    const auto hull = ConvexHull(vs);
    ASSERT_EQ(hull.size(), 3);
    ASSERT_EQ(hull[0], Point2D<int>(1, 1));
    ASSERT_EQ(hull[1], Point2D<int>(3, 1));
    ASSERT_EQ(hull[2], Point2D<int>(2, 2));
  }
  {
    vector<Point2D<int>> vs{{1, 1}, {2, 2}, {2, 0}, {2, 4}, {4, 2}};
    const auto hull = ConvexHull(vs);
    ASSERT_EQ(hull.size(), 4);
    ASSERT_EQ(hull[0], Point2D<int>(1, 1));
    ASSERT_EQ(hull[1], Point2D<int>(2, 0));
    ASSERT_EQ(hull[2], Point2D<int>(4, 2));
    ASSERT_EQ(hull[3], Point2D<int>(2, 4));
  }
}
}  // namespace algo::geom
