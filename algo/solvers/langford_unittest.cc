#include <gtest/gtest.h>

#include "solvers/langford.h"

#include <cstdint>

using namespace algo::solvers;
using namespace std;

namespace {
bool IsGood(int n, const vector<int>& xs) {
  if (xs.size() != static_cast<size_t>(2 * n))
    return false;

  vector<bool> found(n + 1);
  for (int i = 0; i < 2 * n; ++i) {
    if (xs[i] < 0)
      continue;

    const auto v = xs[i];
    if (v < 1 || v > n || found[v])
      return false;
    if (i + v + 1 >= 2 * n || xs[i + v + 1] != -v)
      return false;
    found[v] = true;
  }
  for (int i = 1; i <= n; ++i) {
    if (!found[i])
      return false;
  }
  return true;
}

uint64_t CountSolutions(int n) {
  uint64_t result = 0;
  Langford{}.Solve(n, [&n, &result](const vector<int>& solution) {
    ASSERT_TRUE(IsGood(n, solution));
    ++result;
  });
  return result;
}
}  // namespace

namespace algo {
TEST(Solvers, Langford) {
  ASSERT_EQ(CountSolutions(0), 1);
  ASSERT_EQ(CountSolutions(1), 0);
  ASSERT_EQ(CountSolutions(2), 0);
  ASSERT_EQ(CountSolutions(3), 2);
  ASSERT_EQ(CountSolutions(4), 2);
  ASSERT_EQ(CountSolutions(5), 0);
  ASSERT_EQ(CountSolutions(6), 0);
  ASSERT_EQ(CountSolutions(7), 52);
  ASSERT_EQ(CountSolutions(8), 300);
  ASSERT_EQ(CountSolutions(9), 0);
  ASSERT_EQ(CountSolutions(10), 0);
  ASSERT_EQ(CountSolutions(11), 35584);
  ASSERT_EQ(CountSolutions(12), 216288);
}
}  // namespace algo
