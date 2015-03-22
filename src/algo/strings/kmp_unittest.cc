#include <gtest/gtest.h>

#include <cstddef>
#include <string>
#include <vector>

#include "algo/strings/kmp.h"

namespace algo {

TEST(KMP, Empty) {
  std::string s;
  std::vector<size_t> p;
  BuildPrefixFunction(s.size(), s.data(), p.data());
}

TEST(KMP, SameLetter) {
  std::string s("aaaaa");
  std::vector<size_t> p(s.size());
  BuildPrefixFunction(s.size(), s.data(), p.data());
  std::vector<size_t> expected = {0, 1, 2, 3, 4};
  ASSERT_EQ(expected, p);
}

TEST(KMP, Small) {
  std::string s("abcdabc");
  std::vector<size_t> p(s.size());
  BuildPrefixFunction(s.size(), s.data(), p.data());
  std::vector<size_t> expected = {0, 0, 0, 0, 1, 2, 3};
  ASSERT_EQ(expected, p);
}

TEST(KMP, EmptyPatternMatching) {
  std::string s("hello");
  std::string p("");

  std::vector<size_t> matches;
  KMP(s.size(), s.data(), p.size(), p.data(), matches);

  std::vector<size_t> expected = {0, 1, 2, 3, 4, 5};
  ASSERT_EQ(expected, matches);
}

TEST(KMP, SampleMatching) {
  std::string s("ababab");
  std::string p("ba");

  std::vector<size_t> matches;
  KMP(s.size(), s.data(), p.size(), p.data(), matches);

  std::vector<size_t> expected = {1, 3};
  ASSERT_EQ(expected, matches);
}

TEST(KMP, SameLetterMatching) {
  std::string s("aaaaaaaaaa");
  std::string p("aaa");

  std::vector<size_t> matches;
  KMP(s.size(), s.data(), p.size(), p.data(), matches);

  std::vector<size_t> expected = {0, 1, 2, 3, 4, 5, 6, 7};
  ASSERT_EQ(expected, matches);
}

TEST(Z, EmptyText) {
  std::string s("");
  std::vector<size_t> z(s.size());
  Z(s.size(), s.data(), z.data());
}

TEST(Z, Samples) {
  {
    std::string s("aaaaa");
    std::vector<size_t> expected = {0, 4, 3, 2, 1};
    std::vector<size_t> z(s.size());
    Z(s.size(), s.data(), z.data());
    ASSERT_EQ(expected, z);
  }
  {
    std::string s("aaabaab");
    std::vector<size_t> expected = {0, 2, 1, 0, 2, 1, 0};
    std::vector<size_t> z(s.size());
    Z(s.size(), s.data(), z.data());
    ASSERT_EQ(expected, z);
  }
  {
    std::string s("abacaba");
    std::vector<size_t> expected = {0, 0, 1, 0, 3, 0, 1};
    std::vector<size_t> z(s.size());
    Z(s.size(), s.data(), z.data());
    ASSERT_EQ(expected, z);
  }
}

}  // namespace algo
