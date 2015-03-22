#include <gtest/gtest.h>

#include <cstring>
#include <cstdio>
#include <vector>

#include "algo/strings/suffix_array.h"

namespace algo {

static_assert(sizeof(uint8_t) == sizeof(char),
              "sizes of char and uint8_t should be equal");

TEST(SuffixArray, Empty) {
  ManberMyers(0, nullptr, nullptr);
}

TEST(SuffixArray, Simple) {
  const char* s = "aaaa";
  const int n = strlen(s) + 1;  // include trailing zero character
  std::vector<size_t> pos(n);
  ManberMyers(n, reinterpret_cast<const uint8_t*>(s), pos.data());

  ASSERT_EQ(4, pos[0]);
  ASSERT_EQ(3, pos[1]);
  ASSERT_EQ(2, pos[2]);
  ASSERT_EQ(1, pos[3]);
  ASSERT_EQ(0, pos[4]);
}

TEST(SuffixArray, Classic) {
  const char* s = "mississippi";
  const int n = strlen(s) + 1;  // include trailing zero character
  std::vector<size_t> pos(n);
  ManberMyers(n, reinterpret_cast<const uint8_t*>(s), pos.data());

  ASSERT_STREQ("", s + pos[0]);
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

TEST(SuffixArray, LCP) {
  const char* s = "ababc";
  const int n = strlen(s) + 1;  // include trailing zero character
  std::vector<size_t> pos(n);
  ManberMyers(n, reinterpret_cast<const uint8_t*>(s), pos.data());

  ASSERT_STREQ("", s + pos[0]);
  ASSERT_STREQ("ababc", s + pos[1]);
  ASSERT_STREQ("abc", s + pos[2]);
  ASSERT_STREQ("babc", s + pos[3]);
  ASSERT_STREQ("bc", s + pos[4]);
  ASSERT_STREQ("c", s + pos[5]);

  std::vector<size_t> rank(n);
  std::vector<size_t> lcp(n - 1);
  Kasai(n,
        reinterpret_cast<const uint8_t*>(s),
        pos.data(),
        rank.data(),
        lcp.data());
  ASSERT_EQ(0, lcp[0]);
  ASSERT_EQ(2, lcp[1]);
  ASSERT_EQ(0, lcp[2]);
  ASSERT_EQ(1, lcp[3]);
  ASSERT_EQ(0, lcp[4]);
}

}  // namespace algo
