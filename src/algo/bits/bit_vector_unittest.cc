#include <gtest/gtest.h>

#include "algo/bits/bit_vector.h"

namespace algo {
TEST(Bits, BitVector) {
  BitVector bits(10);
  ASSERT_EQ(static_cast<size_t>(10), bits.NumBits());
  for (size_t i = 0; i < bits.NumBits(); ++i)
    ASSERT_FALSE(bits.Test(i));

  bits.Set(0);
  bits.Set(3);
  bits.Set(5);
  bits.Set(6);
  bits.Set(9);

  for (size_t i : {1, 2, 4, 7, 8})
    ASSERT_FALSE(bits.Test(i));
  for (size_t i : {0, 3, 5, 6, 9})
    ASSERT_TRUE(bits.Test(i));
  ASSERT_EQ(static_cast<uint64_t>(0b1001101001), bits.Block(0));

  bits.Clear(0);
  bits.Clear(1);
  bits.Clear(5);

  for (size_t i : {0, 1, 2, 4, 5, 7, 8})
    ASSERT_FALSE(bits.Test(i));
  for (size_t i : {3, 6, 9})
    ASSERT_TRUE(bits.Test(i));
  ASSERT_EQ(static_cast<uint64_t>(0b1001001000), bits.Block(0));
}
}  // namespace algo
