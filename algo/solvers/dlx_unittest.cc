#include <gtest/gtest.h>

#include "solvers/dlx.h"
#include "solvers/langford.h"

#include <algorithm>
#include <limits>
#include <numeric>
#include <vector>

using namespace algo::solvers;
using namespace std;

namespace {
const auto INF = numeric_limits<uint32_t>::max();

template <typename T>
void Sort(vector<T>& vs) {
  sort(vs.begin(), vs.end());
}

template <typename T>
void SortUnique(vector<T>& vs) {
  Sort(vs);
  vs.erase(unique(vs.begin(), vs.end()), vs.end());
}

vector<vector<int>> LangfordBaseline(uint32_t size) {
  vector<vector<int>> solutions;
  Langford{}.Solve(size, [&](vector<int> solution) { solutions.emplace_back(move(solution)); });
  Sort(solutions);
  return solutions;
}

vector<vector<int>> LangfordDLX(uint32_t size) {
  enum { ITEM, POS, NUM_DIMS };

  DLX::DimTask<NUM_DIMS> task{/* items */ size, /* positions */ 2 * size};

  for (uint32_t i = 0; i < size; ++i) {
    for (uint32_t j = 0; j + i + 2 < 2 * size; ++j)
      task.AddOption().SetItems<ITEM>(i).SetItems<POS>(j, j + i + 2);
  }

  vector<vector<int>> solutions;

  DLX::Solve(task, [&](const uint32_t* options, uint32_t n) {
    ASSERT_EQ(n, size);
    vector<int> solution(2 * size);
    for (uint32_t i = 0; i < n; ++i) {
      const auto& option = task.GetOption(options[i]);
      const int value = option[ITEM][0] + 1;
      solution[option[POS][0]] = value;
      solution[option[POS][1]] = -value;
    }
    solutions.emplace_back(move(solution));
  });

  Sort(solutions);
  return solutions;
}

TEST(DLX, Smoke0) {
  DLX::Task task{/* numItems= */ 0};
  bool called = false;
  DLX::Solve(task, [&](const uint32_t* options, uint32_t n) {
    ASSERT_EQ(n, 0);
    called = true;
  });
  ASSERT_TRUE(called);
}

TEST(DLX, Smoke1) {
  DLX::Task task{/* numItems= */ 1};
  bool called = false;
  DLX::Solve(task, [&](const uint32_t* options, uint32_t n) {
    ASSERT_EQ(n, 0);
    called = true;
  });
  ASSERT_FALSE(called);
}

TEST(DLX, Simple) {
  DLX::Task task{/* numItems= */ 7};
  task.AddOption({2, 4});
  task.AddOption({0, 3, 6});
  task.AddOption({1, 2, 5});
  task.AddOption({0, 3, 5});
  task.AddOption({1, 6});

  vector<vector<uint32_t>> solutions;
  DLX::Solve(task, [&](const uint32_t* options, uint32_t n) {
    vector<uint32_t> solution(options, options + n);
    solutions.emplace_back(move(solution));
  });
  ASSERT_EQ(solutions.size(), 1);

  auto& actual = solutions[0];
  Sort(actual);

  const vector<uint32_t> expected{0, 3, 4};
  ASSERT_EQ(expected, actual);
}

TEST(DLX, Permutations) {
  const uint32_t SIZE = 5;

  DLX::Task task{/* numItems= */ 2 * SIZE};
  for (uint32_t i = 0; i < SIZE; ++i) {
    for (uint32_t j = 0; j < SIZE; ++j) {
      // This option means placement of value i on the j-th position.
      task.AddOption({i, SIZE + j});
    }
  }

  vector<vector<uint32_t>> actual;
  DLX::Solve(task, [&](const uint32_t* options, uint32_t n) {
    ASSERT_EQ(n, SIZE);

    vector<uint32_t> permutation(SIZE);
    for (uint32_t i = 0; i < n; ++i) {
      const auto& option = task.GetOption(options[i]);
      permutation[option[1] - SIZE] = option[0];
    }
    actual.emplace_back(move(permutation));
  });
  Sort(actual);

  vector<vector<uint32_t>> expected;
  {
    vector<uint32_t> permutation(SIZE);
    iota(permutation.begin(), permutation.end(), static_cast<uint32_t>(0));
    do {
      expected.push_back(permutation);
    } while (next_permutation(permutation.begin(), permutation.end()));
  }

  ASSERT_EQ(expected.size(), actual.size());
  for (size_t i = 0; i < expected.size(); ++i)
    ASSERT_EQ(expected[i], actual[i]);
}

TEST(DLX, Langford) {
  const uint32_t MAX_SIZE = 12;

  for (uint32_t size = 0; size <= MAX_SIZE; ++size) {
    const auto expected = LangfordBaseline(size);
    const auto actual = LangfordDLX(size);
    ASSERT_EQ(expected.size(), actual.size()) << "size check failed at size " << size;
    ASSERT_EQ(expected, actual) << "contents check failed at size " << size;
  }
}

TEST(DLX, NQueens) {
  enum { ROW, COL, LR_DIAG, RL_DIAG, NUM_DIMS };

  const int32_t BOARD_SIZE = 8;

  DLX::DimTask<NUM_DIMS> task{/* rows */ BOARD_SIZE,
                              /* cols */ BOARD_SIZE,
                              /* ↘ diags */ 2 * BOARD_SIZE - 1,
                              /* ↙ diags */ 2 * BOARD_SIZE - 1};
  task.SetSlack<LR_DIAG>();
  task.SetSlack<RL_DIAG>();

  for (int32_t r = 0; r < BOARD_SIZE; ++r) {
    for (int32_t c = 0; c < BOARD_SIZE; ++c)
      task.AddOption().SetItems<ROW>(r).SetItems<COL>(c).SetItems<LR_DIAG>(r - c).SetItems<RL_DIAG>(r + c);
  }

  vector<vector<uint32_t>> solutions;

  DLX::Solve(task, [&](const uint32_t* options, uint32_t n) {
    ASSERT_EQ(n, BOARD_SIZE);

    vector<uint32_t> positions(n, INF);
    for (uint32_t i = 0; i < n; ++i) {
      const auto& option = task.GetOption(options[i]);
      const auto r = option[ROW][0];
      const auto c = option[COL][0];
      ASSERT_EQ(positions[r], INF);
      positions[r] = c;
    }

    for (uint32_t i = 0; i < n; ++i) {
      for (uint32_t j = 0; j < i; ++j) {
        ASSERT_TRUE(positions[i] != positions[j]);
        ASSERT_TRUE(positions[i] - positions[j] != i - j);
        ASSERT_TRUE(positions[i] - positions[j] != j - i);
      }
    }

    solutions.emplace_back(move(positions));
  });

  ASSERT_EQ(solutions.size(), 92);
  SortUnique(solutions);
  ASSERT_EQ(solutions.size(), 92);
}
}  // namespace
