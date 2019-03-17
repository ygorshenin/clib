#include <gtest/gtest.h>

#include "sequences/median.h"

#include <vector>

using namespace algo;
using namespace std;

namespace {
TEST(Median, Smoke) {
  {
    const vector<int> a = {100};
    const vector<int> b = {};
    ASSERT_EQ(100, GetKth(a, b, 0));
    ASSERT_EQ(100, GetKth(b, a, 0));
  }

  {
    // The merged sequence is: 1, 2, 3, 4, 5, 6, 7, 8.
    const vector<int> a = {2, 4, 7};
    const vector<int> b = {1, 3, 5, 6, 8};
    for (int i = 0; i < 8; ++i) {
      ASSERT_EQ(i + 1, GetKth(a, b, i));
      ASSERT_EQ(i + 1, GetKth(b, a, i));
    }
  }
}
}  // namespace
