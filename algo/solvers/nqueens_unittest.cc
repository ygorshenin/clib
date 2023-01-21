#include <gtest/gtest.h>

#include "solvers/nqueens.h"

#include <array>
#include <cstdint>
using namespace algo::solvers;
using namespace std;

namespace {
uint64_t CountSolutions(uint8_t n) {
  uint64_t result = 0;
  NQueens{}.Solve(n, [&result](const NQueens::Candidates& /* candidates */) { ++result; });
  return result;
}
}  // namespace

namespace algo {
TEST(Solvers, NQueens) {
  ASSERT_EQ(CountSolutions(0), 1);
  ASSERT_EQ(CountSolutions(1), 1);
  ASSERT_EQ(CountSolutions(2), 0);
  ASSERT_EQ(CountSolutions(3), 0);
  ASSERT_EQ(CountSolutions(4), 2);
  ASSERT_EQ(CountSolutions(5), 10);
  ASSERT_EQ(CountSolutions(6), 4);
  ASSERT_EQ(CountSolutions(7), 40);
  ASSERT_EQ(CountSolutions(8), 92);
  ASSERT_EQ(CountSolutions(9), 352);
  ASSERT_EQ(CountSolutions(10), 724);
  ASSERT_EQ(CountSolutions(11), 2'680);
  ASSERT_EQ(CountSolutions(12), 14'200);
  ASSERT_EQ(CountSolutions(13), 73'712);
  ASSERT_EQ(CountSolutions(14), 365'596);
  // ASSERT_EQ(CountSolutions(15), 2'279'184);
  // ASSERT_EQ(CountSolutions(16), 14'772'512);
  // ASSERT_EQ(CountSolutions(17), 95'815'104);
}
}  // namespace algo
