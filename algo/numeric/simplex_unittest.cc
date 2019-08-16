#include <gtest/gtest.h>

#include "numeric/matrix.h"
#include "numeric/simplex.h"

#include <vector>

using namespace algo;
using namespace std;

namespace {
template <typename YS>
double Dot(const std::vector<double>& xs, const YS& ys) {
  double result = 0;
  for (size_t i = 0; i < xs.size(); ++i)
    result += xs[i] * ys[i];
  return result;
}

TEST(Simplex, Basic) {
  double as[][3] = {{3, -1, 5}, {-1, 2, -1}, {2, 4, 1}};
  const vector<double> bs{8, 1, 5};

  // 3 * x[0] - x[1] + 5 * x[2] <= 8
  //    -x[0] + 2 * x[1] - x[2] <= 1
  // 2 * x[0] + 4 * x[1] + x[2] <= 5
  // x[0] + 3 * x[1] - x[2] -> max

  const vector<double> fs{1, 3, -1};

  Simplex simplex{MatrixAdapter<double>(3, 3, &as[0][0]), bs, fs};

  double target;
  vector<double> xs;
  const auto success = simplex.Solve(target, xs);

  ASSERT_TRUE(success);
  ASSERT_DOUBLE_EQ(target, 27.0 / 8.0);
  ASSERT_EQ(xs.size(), fs.size());

  ASSERT_DOUBLE_EQ(Dot(xs, fs), 27.0 / 8.0);
  for (size_t i = 0; i < xs.size(); ++i)
    ASSERT_GE(xs[i], -Simplex::kEps);
  for (size_t i = 0; i < bs.size(); ++i)
    ASSERT_LE(Dot(xs, as[i]), bs[i] + Simplex::kEps);
}

TEST(Simplex, Infinite) {
  Matrix<double> as(0, 1);
  const vector<double> bs;
  const vector<double> fs{1};

  Simplex simplex{as, bs, fs};

  double target;
  vector<double> xs;
  const auto success = simplex.Solve(target, xs);
  ASSERT_FALSE(success);
}

TEST(Simplex, Any) {
  Matrix<double> as(0, 1);
  const vector<double> bs;
  const vector<double> fs{0};

  Simplex simplex{as, bs, fs};

  double target;
  vector<double> xs;
  const auto success = simplex.Solve(target, xs);
  ASSERT_TRUE(success);
  ASSERT_DOUBLE_EQ(target, 0);

  ASSERT_EQ(xs.size(), 1);
  ASSERT_GE(xs[0], -Simplex::kEps);
}

TEST(Simplex, Simple) {
  double as[][3] = {{1, 1, 3}, {2, 2, 5}, {4, 1, 2}};
  const vector<double> bs{30, 24, 36};
  const vector<double> fs{3, 1, 2};

  Simplex simplex{MatrixAdapter<double>(3, 3, &as[0][0]), bs, fs};

  double target;
  vector<double> xs;
  const auto success = simplex.Solve(target, xs);

  ASSERT_TRUE(success);
  ASSERT_DOUBLE_EQ(target, 28);
}
}  // namespace
