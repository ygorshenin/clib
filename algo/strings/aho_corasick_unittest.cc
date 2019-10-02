#include <gtest/gtest.h>

#include "strings/aho_corasick.h"

using namespace algo;

namespace {
TEST(AhoCorasick, Smoke) {
  AhoCorasick ac;
  ac.Add("cd");
  ac.Add("f");
  ac.Add("kl");
  ac.Build();

  auto curr = ac.Begin();
  ASSERT_FALSE(curr.Move('a'));
  ASSERT_FALSE(curr.Move('b'));
  ASSERT_FALSE(curr.Move('c'));
  ASSERT_TRUE(curr.Move('d'));
  ASSERT_FALSE(curr.Move('e'));
  ASSERT_TRUE(curr.Move('f'));
  ASSERT_FALSE(curr.Move('g'));
  ASSERT_FALSE(curr.Move('h'));
  ASSERT_FALSE(curr.Move('i'));
  ASSERT_FALSE(curr.Move('j'));
  ASSERT_FALSE(curr.Move('k'));
  ASSERT_TRUE(curr.Move('l'));
}

TEST(AhoCorasick, ABA) {
  AhoCorasick ac;
  ac.Add("aabc");
  ac.Add("ab");
  ac.Build();

  auto curr = ac.Begin();
  ASSERT_FALSE(curr.Move('a'));
  ASSERT_FALSE(curr.Move('a'));
  ASSERT_TRUE(curr.Move('b'));
}
} // namespace
