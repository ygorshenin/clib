#include <gtest/gtest.h>

#include "algo/bits/bit_vector.h"
#include "algo/bits/rs_table.h"
#include "algo/math/math.h"

namespace algo {
TEST(Bits, RSTable_Smoke) {
  {
    BitVector bv(0);
    RSTable rs(bv);
    ASSERT_EQ(0, rs.Rank(0));
  }

  {
    BitVector bv(100);
    RSTable rs(bv);
    for (uint64_t i = 0; i <= bv.NumBits(); ++i)
      ASSERT_EQ(0, rs.Rank(i));
  }

  {
    BitVector bv(1025);
    for (uint64_t i = 0; i < bv.NumBits(); ++i)
      bv.Set(i);

    RSTable rs(bv);
    for (uint64_t i = 0; i <= bv.NumBits(); ++i)
      ASSERT_EQ(i, rs.Rank(i));
  }

  {
    BitVector bv(3000);
    for (uint64_t i = 0; i < bv.NumBits(); ++i) {
      if (IsPrime(i))
        bv.Set(i);
    }

    RSTable rs(bv);
    uint64_t rank = 0;
    for (uint64_t i = 0; i < bv.NumBits(); ++i) {
      ASSERT_EQ(rank, rs.Rank(i));
      rank += bv.Test(i);
    }
    ASSERT_EQ(rank, rs.Rank(bv.NumBits()));
  }
}
} // namespace algo
