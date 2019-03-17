#include <gtest/gtest.h>

#include "numeric/fft.h"

#include <vector>

using namespace algo;

namespace {
TEST(Butterfly, Smoke) {
  {
    std::vector<Complex> xs;
    std::vector<Complex> ys;
    Butterfly(xs, ys);
    ASSERT_TRUE(ys.empty());
  }

  {
    std::vector<Complex> xs = {{0, 1, 2, 3, 4, 5, 6, 7}};
    std::vector<Complex> ys(xs.size());
    Butterfly(xs, ys);
    ASSERT_EQ(ys, (std::vector<Complex>{{0, 4, 2, 6, 1, 5, 3, 7}}));
  }
}
}  // namespace
