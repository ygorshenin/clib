#include "bits/rs_table.h"

#include "bits/bits.h"

#include <algorithm>
#include <cassert>

namespace algo {
RSTable::RSTable(const BitVector& bv) : m_bv{bv} {
  const uint64_t numSuperBlocks = (bv.NumBlocks() + 7) / 8;
  m_superBlocks.resize(numSuperBlocks);

  uint64_t rank = 0;
  m_superBlocks[0].m_startRank = rank;
  for (uint64_t i = 0; i < bv.NumBlocks(); ++i) {
    const uint64_t p = i + 1;
    const uint64_t superBlock = p / 8;
    const uint64_t superBlockOffset = p % 8;

    rank += PopCount(bv.Block(i));

    auto& sp = m_superBlocks[superBlock];
    if (superBlockOffset == 0) {
      sp.m_startRank = rank;
    } else {
      const uint64_t offset = 9 * (superBlockOffset - 1);
      sp.m_otherRanks |= (rank - sp.m_startRank) << offset;
    }
  }
}

uint64_t RSTable::Rank1(uint64_t n) const {
  assert(n <= m_bv.NumBits());
  const uint64_t block = n / 64;
  const uint64_t blockOffset = n % 64;

  const uint64_t superBlock = block / 8;
  const uint64_t superBlockOffset = block % 8 - 1;

  assert(superBlock < m_superBlocks.size());
  const auto& sb = m_superBlocks[superBlock];

  uint64_t rank = sb.m_startRank;

  // Following code tries to get rid of branching. When 0 <=
  // super_block_offset < 7, (super_block_offset >> 60) & 8 equals to
  // zero, therefore offset is correct. On the other case, when
  // super_block_offset is -1, (super_block_offset >> 60) & 8 equals
  // to 8, so offset will be (-1 + 8) * 9 = 63.  As we are assuming
  // here that only first 63 bits of other_ranks_ are busy, zero will
  // be added to the rank.
  const uint64_t offset = (superBlockOffset + ((superBlockOffset >> 60) & 8)) * 9;
  rank += (sb.m_otherRanks >> offset) & 0x1FF;

  const uint64_t mask = (static_cast<uint64_t>(1) << blockOffset) - 1;
  rank += PopCount(m_bv.Block(block) & mask);

  return rank;
}
}  // namespace algo
