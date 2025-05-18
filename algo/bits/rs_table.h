#pragma once

#include "bits/bit_vector.h"

#include <climits>
#include <cstdint>
#include <vector>

namespace algo {
class RSTable {
public:
  static_assert(CHAR_BIT == 8, "");

  explicit RSTable(const BitVector& bv);

  // Returns number of zeroes among the first |n| bits.
  uint64_t Rank0(uint64_t n) const { return n - Rank1(n); }

  // Returns number of ones among the first |n| bits.
  uint64_t Rank1(uint64_t n) const;

private:
  struct SuperBlock {
    uint64_t m_startRank{};
    uint64_t m_otherRanks{};
  };

  const BitVector& m_bv;
  std::vector<SuperBlock> m_superBlocks;
};
}  // namespace algo
