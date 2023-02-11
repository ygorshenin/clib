#include <gtest/gtest.h>

#include "solvers/sudoku.h"

using namespace algo::solvers;
using namespace std;

namespace {
TEST(Sudoku, Test0) {
  const Sudoku::Board board{{{0, 0, 3, 0, 1, 0, 0, 0, 0},
                             {4, 1, 5, 0, 0, 0, 0, 9, 0},
                             {2, 0, 6, 5, 0, 0, 3, 0, 0},
                             {5, 0, 0, 0, 8, 0, 0, 0, 9},
                             {0, 7, 0, 9, 0, 0, 0, 3, 2},
                             {0, 3, 8, 0, 0, 4, 0, 6, 0},
                             {0, 0, 0, 2, 6, 0, 4, 0, 3},
                             {0, 0, 0, 3, 0, 0, 0, 0, 8},
                             {3, 2, 0, 0, 0, 7, 9, 5, 0}}};

  uint32_t found = 0;
  Sudoku::Solve(board, [&](const Sudoku::Board& solution) {
    ASSERT_TRUE(Sudoku::IsGood(solution));
    ++found;
  });
  ASSERT_EQ(found, 1);
}
}  // namespace
