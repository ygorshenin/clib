#include <gtest/gtest.h>

#include "solvers/sudoku.h"

using namespace algo::solvers;
using namespace std;

namespace {
struct IsSolvableException {};

bool IsSolvable(const Sudoku::Board& board) {
  try {
    Sudoku::Solve(board, [&](const Sudoku::Board& solution) { throw IsSolvableException{}; });
  } catch (const IsSolvableException&) {
    return true;
  }
  return false;
}

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
    ASSERT_TRUE(Sudoku::IsGood</* IsComplete= */ true>(solution));
    ++found;
  });
  ASSERT_EQ(found, 1);
}

TEST(Sudoku, Efficiency9) {
  Sudoku::Board board{};
  ASSERT_TRUE(IsSolvable(board));
  for (size_t i = 0; i < board.size(); ++i) {
    for (size_t j = 0; j < board[i].size(); ++j) {
      for (int v = 1; v <= 9; ++v) {
        board[i][j] = v;
        if (IsSolvable(board))
          break;
      }
      ASSERT_TRUE(IsSolvable(board));
    }
  }
}

TEST(Sudoku, Efficiency10) {
  const Sudoku::Board board{{{0, 0, 0, 0, 0, 0, 6, 8, 0},
                             {0, 0, 0, 0, 7, 3, 0, 0, 9},
                             {3, 0, 9, 0, 0, 0, 0, 4, 5},
                             {4, 9, 0, 0, 0, 0, 0, 0, 0},
                             {8, 0, 3, 0, 5, 0, 9, 0, 2},
                             {0, 0, 0, 0, 0, 0, 0, 3, 6},
                             {9, 6, 0, 0, 0, 0, 3, 0, 8},
                             {7, 0, 0, 6, 8, 0, 0, 0, 0},
                             {0, 2, 8, 0, 0, 0, 0, 0, 0}}};

  uint32_t found = 0;
  Sudoku::Solve(board, [&](const Sudoku::Board& solution) {
    ASSERT_TRUE(Sudoku::IsGood</* IsComplete= */ true>(solution));
    ++found;
  });
  ASSERT_EQ(found, 1);
}

TEST(Sudoku, Efficiency11) {
  Sudoku::Board board{{{0, 6, 4, 0, 0, 0, 7, 0, 0},
                       {0, 0, 0, 0, 2, 0, 0, 3, 6},
                       {0, 0, 1, 0, 0, 0, 0, 0, 0},
                       {2, 3, 0, 0, 8, 0, 0, 0, 0},
                       {0, 0, 0, 7, 0, 0, 1, 0, 4},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {9, 0, 0, 0, 0, 0, 0, 0, 0},
                       {8, 0, 0, 0, 0, 0, 0, 2, 0},
                       {0, 0, 0, 4, 0, 0, 0, 0, 0}}};
  ASSERT_TRUE(IsSolvable(board));
  for (size_t i = 0; i < board.size(); ++i) {
    for (size_t j = 0; j < board[i].size(); ++j) {
      if (board[i][j] != 0)
        continue;
      for (int v = 1; v <= 9; ++v) {
        board[i][j] = v;
        if (IsSolvable(board))
          break;
      }
      ASSERT_TRUE(IsSolvable(board));
    }
  }
}
}  // namespace
