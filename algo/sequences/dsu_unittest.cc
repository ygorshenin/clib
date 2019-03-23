#include <gtest/gtest.h>

#include "dsu.h"

using namespace algo;

namespace {
TEST(DSU, Smoke) { DSU dsu{0}; }

TEST(DSU, Small) {
  DSU dsu{5};
  for (size_t i = 0; i < 5; ++i)
    ASSERT_EQ(dsu.GetParent(i), i);

  ASSERT_FALSE(dsu.Union(2, 2));
  ASSERT_TRUE(dsu.Union(2, 3));
  ASSERT_EQ(dsu.GetParent(2), dsu.GetParent(3));
}
}  // namespace
