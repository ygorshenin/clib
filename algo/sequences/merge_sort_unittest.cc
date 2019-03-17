#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "sequences/merge_sort.h"

TEST(Merge, SortSmoke) {
  std::vector<int> data;
  algo::MergeSort(data.size(), data.data());
}

TEST(Merge, Sort) {
  std::vector<int> data = {6, 0, 8, 1, 2, 4, 5, 3, -1, 15};

  std::vector<int> expected(data.begin(), data.end());
  std::sort(expected.begin(), expected.end());

  algo::MergeSort(data.size(), data.data(), algo::SortOptions(4 /* cache_size */, 1 /* cache_line_size */));

  ASSERT_EQ(data, expected);
}
