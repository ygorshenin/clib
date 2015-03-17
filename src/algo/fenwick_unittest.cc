#include <gtest/gtest.h>

#include "algo/fenwick.h"

namespace algo {

TEST(Fenwick, Empty) {
  Fenwick<int> fenwick(0);
  ASSERT_EQ(static_cast<size_t>(0), fenwick.size());
}

TEST(Fenwick, Smoke) {
  static const size_t kSize = 8;
  const int values[kSize] = {6, 4, 1, 2, 3, 5, 7, 9};
  ASSERT_EQ(kSize, sizeof(values) / sizeof(*values));

  Fenwick<int> fenwick(kSize);
  for (size_t index = 0; index < kSize; ++index)
    fenwick.Add(index, values[index]);

  // Checks all prefix sums.
  ASSERT_EQ(0, fenwick.Sum(0));

  int sum = 0;
  for (size_t to = 1; to <= kSize; ++to) {
    sum += values[to - 1];
    ASSERT_EQ(sum, fenwick.Sum(to));
  }

  // Checks all sub-interval sums.
  for (size_t from = 0; from <= kSize; ++from) {
    ASSERT_EQ(0, fenwick.Sum(from, from));
    int sum = 0;
    for (size_t to = from + 1; to <= kSize; ++to) {
      sum += values[to - 1];
      ASSERT_EQ(sum, fenwick.Sum(from, to));
    }
  }
}

}  // namespace algo
