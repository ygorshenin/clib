#include <gtest/gtest.h>

#include "bits/dictionary.h"

namespace algo {
TEST(Bits, DictionaryLeaf) {
  DictionaryLeaf dict;

  ASSERT_FALSE(dict.Get(3));
  ASSERT_FALSE(dict.Get(10));
  ASSERT_FALSE(dict.Get(42));
  ASSERT_FALSE(dict.Min().has_value());
  ASSERT_FALSE(dict.Max().has_value());
  ASSERT_FALSE(dict.Succ(10).has_value());
  ASSERT_FALSE(dict.Pred(10).has_value());

  dict.Set(10, true);
  ASSERT_FALSE(dict.Get(3));
  ASSERT_TRUE(dict.Get(10));
  ASSERT_FALSE(dict.Get(42));
  ASSERT_EQ(dict.Min(), 10);
  ASSERT_EQ(dict.Max(), 10);
  ASSERT_FALSE(dict.Succ(10).has_value());
  ASSERT_FALSE(dict.Pred(10).has_value());

  dict.Set(3, true);
  ASSERT_TRUE(dict.Get(3));
  ASSERT_TRUE(dict.Get(10));
  ASSERT_FALSE(dict.Get(42));
  ASSERT_EQ(dict.Min(), 3);
  ASSERT_EQ(dict.Max(), 10);
  ASSERT_FALSE(dict.Succ(10).has_value());
  ASSERT_EQ(dict.Pred(10), 3);

  dict.Set(42, true);
  ASSERT_TRUE(dict.Get(3));
  ASSERT_TRUE(dict.Get(10));
  ASSERT_TRUE(dict.Get(42));
  ASSERT_EQ(dict.Min(), 3);
  ASSERT_EQ(dict.Max(), 42);
  ASSERT_EQ(dict.Succ(10), 42);
  ASSERT_EQ(dict.Pred(10), 3);

  dict.Set(3, false);
  ASSERT_FALSE(dict.Get(3));
  ASSERT_TRUE(dict.Get(10));
  ASSERT_TRUE(dict.Get(42));
  ASSERT_EQ(dict.Min(), 10);
  ASSERT_EQ(dict.Max(), 42);
  ASSERT_EQ(dict.Succ(10), 42);
  ASSERT_FALSE(dict.Pred(10).has_value());

  dict.Set(10, false);
  ASSERT_FALSE(dict.Get(3));
  ASSERT_FALSE(dict.Get(10));
  ASSERT_TRUE(dict.Get(42));
  ASSERT_EQ(dict.Min(), 42);
  ASSERT_EQ(dict.Max(), 42);
  ASSERT_EQ(dict.Succ(10), 42);
  ASSERT_FALSE(dict.Pred(10).has_value());

  dict.Set(42, false);
  ASSERT_FALSE(dict.Get(3));
  ASSERT_FALSE(dict.Get(10));
  ASSERT_FALSE(dict.Get(42));
  ASSERT_FALSE(dict.Min().has_value());
  ASSERT_FALSE(dict.Max().has_value());
  ASSERT_FALSE(dict.Succ(10).has_value());
  ASSERT_FALSE(dict.Pred(10).has_value());
}

TEST(Bits, Dictionary) {
  Dictionary<20> dict;
  dict.Set(1, true);
  dict.Set(3, true);
  dict.Set(5, true);

  ASSERT_EQ(dict.Succ(2), 3);
  ASSERT_EQ(dict.Pred(4), 3);
  ASSERT_FALSE(dict.Pred(1).has_value());

  dict.Set(4, true);
  dict.Set(3, false);

  ASSERT_TRUE(dict.Get(4));
  ASSERT_FALSE(dict.Get(3));

  ASSERT_EQ(dict.Succ(2), 4);
  ASSERT_EQ(dict.Pred(4), 1);
}
}  // namespace algo
