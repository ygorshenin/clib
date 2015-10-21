#include <gtest/gtest.h>

#include "algo/bits/bits.h"

namespace algo {
TEST(Bits, PopCount) {
  ASSERT_EQ(static_cast<uint8_t>(0), PopCount(0x0));
  ASSERT_EQ(static_cast<uint8_t>(1), PopCount(0x1));
  ASSERT_EQ(static_cast<uint8_t>(32), PopCount(0xAAAAAAAA55555555));
  ASSERT_EQ(static_cast<uint8_t>(64), PopCount(0xFFFFFFFFFFFFFFFF));
}

TEST(Bits, CeilLog) {
  ASSERT_EQ(static_cast<uint8_t>(0), CeilLog(0x0));
  ASSERT_EQ(static_cast<uint8_t>(0), CeilLog(0x1));
  ASSERT_EQ(static_cast<uint8_t>(1), CeilLog(0x2));
  ASSERT_EQ(static_cast<uint8_t>(1), CeilLog(0x3));
  ASSERT_EQ(static_cast<uint8_t>(2), CeilLog(0x4));

  ASSERT_EQ(static_cast<uint8_t>(6), CeilLog(0x7f));
  ASSERT_EQ(static_cast<uint8_t>(7), CeilLog(0x80));
  ASSERT_EQ(static_cast<uint8_t>(31), CeilLog(0xFFFFFFFF));
  ASSERT_EQ(static_cast<uint8_t>(63), CeilLog(0xFFFFFFFFFFFFFFFF));
}
}  // namespace algo
