#include <gtest/gtest.h>

#include <algorithm>

#include "sequences/segtree.h"

namespace algo {

template <>
int Mappend<int>(const int& lhs, const int& rhs) {
  return lhs + rhs;
}

TEST(SegTree, Simple) {
  const size_t size = 12;
  int values[size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  SegTree<int> seg_tree(size);

  ASSERT_EQ(size, seg_tree.size());
  for (size_t i = 0; i < size; ++i)
    seg_tree.Set(i, values[i]);
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(values[i], seg_tree.Get(i));
    int sum = 0;
    for (size_t j = i; j < size; ++j) {
      ASSERT_EQ(sum, seg_tree.Get(i, j));
      sum += values[j];
    }
  }

  std::reverse(values, values + size);
  for (size_t i = 0; i < size; ++i)
    seg_tree.Set(i, values[i]);
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(values[i], seg_tree.Get(i));
    int sum = 0;
    for (size_t j = i; j < size; ++j) {
      ASSERT_EQ(sum, seg_tree.Get(i, j));
      sum += values[j];
    }
  }
}

}  // namespace algo
