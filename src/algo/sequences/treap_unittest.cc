#include <gtest/gtest.h>

#include <cstdlib>
#include <string>

#include "algo/sequences/treap.h"

TEST(Treap, Smoke) {
  srand(10 /* seed */);
  algo::TreapMap<int /* key */, std::string /* value */, int /* priority */> treap;

  treap.Insert(1, "1", rand());
  treap.Insert(2, "2", rand());
  treap.Insert(3, "3", rand());
  ASSERT_EQ(static_cast<size_t>(3), treap.Size());

  ASSERT_TRUE(treap.Contains(1));
  ASSERT_TRUE(treap.Contains(2));
  ASSERT_TRUE(treap.Contains(3));
  ASSERT_FALSE(treap.Contains(0));
  ASSERT_FALSE(treap.Contains(4));

  ASSERT_EQ("1", treap.Get(1));
  ASSERT_EQ("2", treap.Get(2));
  ASSERT_EQ("3", treap.Get(3));

  ASSERT_TRUE(treap.Erase(1));
  ASSERT_EQ(static_cast<size_t>(2), treap.Size());

  ASSERT_FALSE(treap.Erase(0));
  ASSERT_EQ(static_cast<size_t>(2), treap.Size());

  ASSERT_TRUE(treap.Erase(2));
  ASSERT_EQ(static_cast<size_t>(1), treap.Size());

  ASSERT_TRUE(treap.Erase(3));
  ASSERT_EQ(static_cast<size_t>(0), treap.Size());
}
