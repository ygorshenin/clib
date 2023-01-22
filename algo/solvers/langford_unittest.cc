#include <gtest/gtest.h>

#include "solvers/langford.h"

#include <cstdint>

using namespace algo::solvers;
using namespace std;

namespace {
uint64_t CountSolutions(uint8_t n) {
  uint64_t result = 0;
  Langford{}.SolveNaive(n, [&result](const vector<int>& /* solution */) { ++result; });
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
