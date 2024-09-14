#include <gtest/gtest.h>

#include <algorithm>

#include "sequences/segtree.h"

namespace algo {
TEST(SegTree, Simple) {
  const size_t size = 12;

  int values[size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  SegTree<int> tree{size};

  ASSERT_EQ(size, tree.Size());
  for (size_t i = 0; i < size; ++i)
    tree.Set(i, values[i]);
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(values[i], tree.Get(i));
    int sum = 0;
    for (size_t j = i; j < size; ++j) {
      ASSERT_EQ(sum, tree.Sum(i, j));
      sum += values[j];
    }
  }

  std::reverse(values, values + size);
  for (size_t i = 0; i < size; ++i)
    tree.Set(i, values[i]);
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(values[i], tree.Get(i));
    int sum = 0;
    for (size_t j = i; j < size; ++j) {
      ASSERT_EQ(sum, tree.Sum(i, j));
      sum += values[j];
    }
  }
}

TEST(SegTree, Color) {
  const size_t size = 6;

  SegTree<int> tree{size};

  tree.Color(0, 4, 1);
  for (size_t i = 0; i < size; ++i) {
    const auto value = tree.Get(i);
    if (i < 4)
      ASSERT_EQ(value, 1);
    else
      ASSERT_EQ(value, 0);
  }

  tree.Color(3, size, 2);
  for (size_t i = 0; i < size; ++i) {
    const auto value = tree.Get(i);
    if (i < 3)
      ASSERT_EQ(value, 1);
    else
      ASSERT_EQ(value, 2);
  }

  tree.Color(2, 4, 3);
  for (size_t i = 0; i < size; ++i) {
    const auto value = tree.Get(i);
    if (i < 2)
      ASSERT_EQ(value, 1);
    else if (i < 4)
      ASSERT_EQ(value, 3);
    else
      ASSERT_EQ(value, 2);
  }
}

TEST(FastSegTree, Simple) {
  const size_t size = 12;

  int values[size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  FastSegTree<int> tree{size};

  ASSERT_EQ(size, tree.Size());
  for (size_t i = 0; i < size; ++i)
    tree.Set(i, values[i]);
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(values[i], tree.Get(i));
    int sum = 0;
    for (size_t j = i; j < size; ++j) {
      ASSERT_EQ(sum, tree.Sum(i, j));
      sum += values[j];
    }
  }

  std::reverse(values, values + size);
  for (size_t i = 0; i < size; ++i)
    tree.Set(i, values[i]);
  for (size_t i = 0; i < size; ++i) {
    ASSERT_EQ(values[i], tree.Get(i));
    int sum = 0;
    for (size_t j = i; j < size; ++j) {
      ASSERT_EQ(sum, tree.Sum(i, j));
      sum += values[j];
    }
  }
}
}  // namespace algo
