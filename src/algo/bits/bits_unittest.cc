#include <gtest/gtest.h>

#include "algo/bits/bits.h"

namespace algo {
TEST(Bits, PopCount) {
  ASSERT_EQ(static_cast<uint8_t>(0), PopCount(0));
  ASSERT_EQ(static_cast<uint8_t>(1), PopCount(1));
  ASSERT_EQ(static_cast<uint8_t>(32), PopCount(0xAAAAAAAA55555555));
  ASSERT_EQ(static_cast<uint8_t>(64), PopCount(0xFFFFFFFFFFFFFFFF));
}
}  // namespace algo