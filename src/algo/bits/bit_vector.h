#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace algo {
class BitVector {
public:
#define BLOCK_OFFSET(bit)                                                      \
  const uint64_t block = bit >> 6;                                             \
  const uint64_t offset = bit & 0x3F

  BitVector(uint64_t num_bits)
      : blocks_(((num_bits + 63) >> 6) + 1), num_bits_(num_bits) {}

  inline void Set(uint64_t bit) {
    assert(bit < num_bits_);
    BLOCK_OFFSET(bit);
    blocks_[block] |= static_cast<uint64_t>(1) << offset;
  }

  inline void Clear(uint64_t bit) {
    assert(bit < num_bits_);
    BLOCK_OFFSET(bit);
    blocks_[block] &= ~(static_cast<uint64_t>(1) << offset);
  }

  inline bool Test(uint64_t bit) const {
    assert(bit < num_bits_);
    BLOCK_OFFSET(bit);
    return blocks_[block] & (static_cast<uint64_t>(1) << offset);
  }

  inline uint64_t &Block(uint64_t block) {
    assert(block < blocks_.size());
    return blocks_[block];
  }

  inline const uint64_t &Block(uint64_t block) const {
    assert(block < blocks_.size());
    return blocks_[block];
  }

  inline uint64_t NumBits() const { return num_bits_; }

  inline uint64_t NumBlocks() const { return blocks_.size(); }

private:
  std::vector<uint64_t> blocks_;
  uint64_t const num_bits_;
};
} // namespace algo
