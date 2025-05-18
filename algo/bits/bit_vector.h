#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace algo {
class BitVector {
public:
#define BLOCK_OFFSET(bit)          \
  const uint64_t block = bit >> 6; \
  const uint64_t offset = bit & 0x3F

  // For efficiency reasons, BitVector stores bits in blocks of 64
  // bits. But there may be one more block, in case when number of
  // bits divides by 64 - this is needed for RSTable, as Rank/Select
  // are defined in terms of positions, and number of positions is 1+
  // number of bits. Therefore, there should be one more zero block
  // for the case when number of bits divides by 64, as there is a
  // position after the last bit that is in this dummy zero block.
  explicit BitVector(uint64_t numBits) : m_blocks(((numBits + 63) >> 6) + (numBits % 64 == 0)), m_numBits(numBits) {}

  void Set(uint64_t bit) {
    assert(bit < m_numBits);
    BLOCK_OFFSET(bit);
    m_blocks[block] |= static_cast<uint64_t>(1) << offset;
  }

  void Clear(uint64_t bit) {
    assert(bit < m_numBits);
    BLOCK_OFFSET(bit);
    m_blocks[block] &= ~(static_cast<uint64_t>(1) << offset);
  }

  bool Test(uint64_t bit) const {
    assert(bit < m_numBits);
    BLOCK_OFFSET(bit);
    return m_blocks[block] & (static_cast<uint64_t>(1) << offset);
  }

  uint64_t& Block(uint64_t block) {
    assert(block < m_blocks.size());
    return m_blocks[block];
  }

  const uint64_t& Block(uint64_t block) const {
    assert(block < m_blocks.size());
    return m_blocks[block];
  }

  uint64_t NumBits() const { return m_numBits; }

  uint64_t NumBlocks() const { return m_blocks.size(); }

private:
  std::vector<uint64_t> m_blocks;
  uint64_t const m_numBits;
};
}  // namespace algo
