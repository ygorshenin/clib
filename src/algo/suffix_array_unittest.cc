#include <gtest/gtest.h>

#include <cstring>
#include <cstdio>
#include <vector>

#include "algo/suffix_array.h"

namespace algo {

TEST(SuffixArray, Empty) {
  SortCyclicShifts(0, nullptr, nullptr);
}

TEST(SuffixArray, Simple) {
  const char* s = "aaaa";
  const int n = strlen(s) + 1;  // include trailing zero character
  std::vector<int> pos(n);
  SortCyclicShifts(n, s, pos.data());

  ASSERT_EQ(4, pos[0]);
  ASSERT_EQ(3, pos[1]);
  ASSERT_EQ(2, pos[2]);
  ASSERT_EQ(1, pos[3]);
  ASSERT_EQ(0, pos[4]);
}

TEST(SuffixArray, Classic) {
  const char* s = "mississippi";
  const int n = strlen(s) + 1;  // include trailing zero character
  std::vector<int> pos(n);
  SortCyclicShifts(n, s, pos.data());

  ASSERT_EQ(n - 1, pos[0]);
  ASSERT_STREQ("i", s + pos[1]);
  ASSERT_STREQ("ippi", s + pos[2]);
  ASSERT_STREQ("issippi", s + pos[3]);
  ASSERT_STREQ("ississippi", s + pos[4]);
  ASSERT_STREQ("mississippi", s + pos[5]);
  ASSERT_STREQ("pi", s + pos[6]);
  ASSERT_STREQ("ppi", s + pos[7]);
  ASSERT_STREQ("sippi", s + pos[8]);
  ASSERT_STREQ("sissippi", s + pos[9]);
  ASSERT_STREQ("ssippi", s + pos[10]);
  ASSERT_STREQ("ssissippi", s + pos[11]);
}

}  // namespace algo
