#pragma once

#include "algo/bits/bit_vector.h"

#include <climits>
#include <cstdint>
#include <vector>

namespace algo {
class RSTable {
public:
  static_assert(CHAR_BIT == 8, "");

  RSTable(const BitVector &bv);

  // Returns number of set bits among the first |n|.
  uint64_t Rank(uint64_t n) const;

private:
  struct SuperBlock {
    uint64_t start_rank_ = 0;
    uint64_t other_ranks_ = 0;
  };

  const BitVector &bv_;
  std::vector<SuperBlock> super_blocks_;
};
} // namespace algo
