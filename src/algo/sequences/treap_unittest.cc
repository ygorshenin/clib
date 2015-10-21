#include <gtest/gtest.h>

#include <cstdlib>
#include <string>

#include "algo/sequences/treap.h"

TEST(Treap, Smoke) {
  srand(0 /* seed */);
  algo::Treap<int /* key */, void /* value */, int /* priority */> treap;

  treap.Insert(1, rand());
  treap.Insert(2, rand());
  treap.Insert(3, rand());
  ASSERT_EQ(static_cast<size_t>(3), treap.Size());

  ASSERT_TRUE(treap.Contains(1));
  ASSERT_TRUE(treap.Contains(2));
  ASSERT_TRUE(treap.Contains(3));
  ASSERT_FALSE(treap.Contains(0));
  ASSERT_FALSE(treap.Contains(4));

  ASSERT_EQ(1, treap.Get(1).key);
  ASSERT_EQ(2, treap.Get(2).key);
  ASSERT_EQ(3, treap.Get(3).key);

  ASSERT_TRUE(treap.Erase(1));
  ASSERT_EQ(static_cast<size_t>(2), treap.Size());

  ASSERT_FALSE(treap.Erase(0));
  ASSERT_EQ(static_cast<size_t>(2), treap.Size());

  ASSERT_TRUE(treap.Erase(2));
  ASSERT_EQ(static_cast<size_t>(1), treap.Size());

  ASSERT_TRUE(treap.Erase(3));
  ASSERT_EQ(static_cast<size_t>(0), treap.Size());
}

TEST(Treap, SameValue) {
  srand(0 /* seed */);
  algo::Treap<int /* key */, int /* value */, int /* priority */> treap;

  ASSERT_EQ(0, treap.Size());

  treap.Insert(1, 1, rand());
  ASSERT_EQ(1, treap.Size());
  ASSERT_EQ(treap.Get(1).value, 1);

  treap.Insert(1, 2, rand());
  ASSERT_EQ(1, treap.Size());
  ASSERT_EQ(treap.Get(1).value, 2);

  treap.Insert(1, 3, rand());
  ASSERT_EQ(1, treap.Size());
  ASSERT_EQ(treap.Get(1).value, 3);

  ASSERT_TRUE(treap.Erase(1));
  ASSERT_EQ(0, treap.Size());
  ASSERT_TRUE(treap.Empty());
}

TEST(Treap, KeyValue) {
  srand(0 /* seed */);
  algo::Treap<int /* key */, std::string /* value */, int /* priority */> treap;

  treap.Insert(1, "1", rand());
  treap.Insert(2, "2", rand());
  treap.Insert(3, "3", rand());

  ASSERT_EQ(treap.Get(1).value, "1");
  ASSERT_EQ(treap.Get(2).value, "2");
  ASSERT_EQ(treap.Get(3).value, "3");
}

TEST(Treap, Perf) {
  const int kNumElems = 100000;

  srand(0 /* seed */);
  algo::Treap<int /* key */, void /* value */, int /* priority */> treap;
  for (int i = 0; i < kNumElems; ++i)
    treap.Insert(i, rand());
  ASSERT_EQ(kNumElems, treap.Size());
  for (int i = 0; i < kNumElems; ++i)
    ASSERT_TRUE(treap.Erase(i));
  ASSERT_EQ(0, treap.Size());
}
