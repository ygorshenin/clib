#include <gtest/gtest.h>

#include "bits/bits.h"

namespace algo {
TEST(Bits, L) {
  ASSERT_EQ(0xFFFFFFFFFFFFFFFF, L(1));
  ASSERT_EQ(0x5555555555555555, L(2));
  ASSERT_EQ(0x9249249249249249, L(3));
  ASSERT_EQ(0x1111111111111111, L(4));
}

TEST(Bits, H) {
  ASSERT_EQ(0xFFFFFFFFFFFFFFFF, H(1));
  ASSERT_EQ(0xAAAAAAAAAAAAAAAA, H(2));
  ASSERT_EQ(0x4924924924924924, H(3));
  ASSERT_EQ(0x8888888888888888, H(4));
}

TEST(Bits, PopCount) {
  ASSERT_EQ(static_cast<uint8_t>(0), PopCount(0x0));
  ASSERT_EQ(static_cast<uint8_t>(1), PopCount(0x1));
  ASSERT_EQ(static_cast<uint8_t>(32), PopCount(0xAAAAAAAA55555555));
  ASSERT_EQ(static_cast<uint8_t>(64), PopCount(0xFFFFFFFFFFFFFFFF));
}

TEST(Bits, LoPos) {
  ASSERT_EQ(static_cast<uint8_t>(64), LoPos(0x0));
  ASSERT_EQ(static_cast<uint8_t>(0), LoPos(0x1));
  ASSERT_EQ(static_cast<uint8_t>(3), LoPos(0x8));
  ASSERT_EQ(static_cast<uint8_t>(63), LoPos(0x8000000000000000));
  ASSERT_EQ(static_cast<uint8_t>(10), LoPos(0x400));
}

TEST(Bits, HiPos) {
  ASSERT_EQ(static_cast<uint8_t>(0), HiPos(0x0));
  ASSERT_EQ(static_cast<uint8_t>(0), HiPos(0x1));
  ASSERT_EQ(static_cast<uint8_t>(1), HiPos(0x2));
  ASSERT_EQ(static_cast<uint8_t>(1), HiPos(0x3));
  ASSERT_EQ(static_cast<uint8_t>(2), HiPos(0x4));

  ASSERT_EQ(static_cast<uint8_t>(6), HiPos(0x7f));
  ASSERT_EQ(static_cast<uint8_t>(7), HiPos(0x80));
  ASSERT_EQ(static_cast<uint8_t>(31), HiPos(0xFFFFFFFF));
  ASSERT_EQ(static_cast<uint8_t>(63), HiPos(0xFFFFFFFFFFFFFFFF));
}
}  // namespace algo
