#include <gtest/gtest.h>

#include <cstddef>
#include <string>
#include <vector>

#include "algo/kmp.h"

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

}  // namespace algo
