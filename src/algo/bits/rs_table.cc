#include "algo/bits/rs_table.h"

#include "algo/bits/bits.h"

#include <algorithm>
#include <cassert>

namespace algo {
RSTable::RSTable(const BitVector &bv) : bv_(bv) {
  const uint64_t num_super_blocks = (bv.NumBlocks() + 7) / 8;
  super_blocks_.resize(num_super_blocks);

  uint64_t rank = 0;
  super_blocks_[0].start_rank_ = rank;
  for (uint64_t i = 0; i < bv.NumBlocks(); ++i) {
    const uint64_t p = i + 1;
    const uint64_t super_block = p / 8;
    const uint64_t super_block_offset = p % 8;

    rank += PopCount(bv.Block(i));

    auto &sp = super_blocks_[super_block];
    if (super_block_offset == 0) {
      sp.start_rank_ = rank;
    } else {
      const uint64_t offset = 9 * (super_block_offset - 1);
      sp.other_ranks_ |= (rank - sp.start_rank_) << offset;
    }
  }
}

uint64_t RSTable::Rank1(uint64_t n) const {
  assert(n <= bv_.NumBits());
  const uint64_t block = n / 64;
  const uint64_t block_offset = n % 64;

  const uint64_t super_block = block / 8;
  const uint64_t super_block_offset = block % 8 - 1;

  assert(super_block < super_blocks_.size());
  const auto &sb = super_blocks_[super_block];

  uint64_t rank = sb.start_rank_;

  // Following code tries to get rid of branching. When 0 <=
  // super_block_offset < 7, (super_block_offset >> 60) & 8 equals to
  // zero, therefore offset is correct. On the other case, when
  // super_block_offset is -1, (super_block_offset >> 60) & 8 equals
  // to 8, so offset will be (-1 + 8) * 9 = 63.  As we are assuming
  // here that only first 63 bits of other_ranks_ are busy, zero will
  // be added to the rank.
  const uint64_t offset =
      (super_block_offset + ((super_block_offset >> 60) & 8)) * 9;
  rank += (sb.other_ranks_ >> offset) & 0x1FF;

  const uint64_t mask = (static_cast<uint64_t>(1) << block_offset) - 1;
  rank += PopCount(bv_.Block(block) & mask);

  return rank;
}
} // namespace algo
