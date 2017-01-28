#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>


namespace algo {
class BitVector {
public:
#define BLOCK_OFFSET(bit)                                                      \
  const uint64_t block = bit >> 6;                                             \
  const uint64_t offset = bit & 0x3F

  BitVector(uint64_t size) : blocks_((size + 63) >> 6), size_(size) {}

  inline void Set(uint64_t bit) {
    BLOCK_OFFSET(bit);
    blocks_[block] |= static_cast<uint64_t>(1) << offset;
  }

  inline void Clear(uint64_t bit) {
    BLOCK_OFFSET(bit);
    blocks_[block] &= ~(static_cast<uint64_t>(1) << offset);
  }

  inline bool Test(uint64_t bit) const {
    BLOCK_OFFSET(bit);
    return blocks_[block] & (static_cast<uint64_t>(1) << offset);
  }

  inline uint64_t Size() const { return size_; }

  inline uint64_t NumBlocks() const { return blocks_.size(); }

  inline uint64_t &Block(uint64_t block) { return blocks_[block]; }

  inline const uint64_t &Block(uint64_t block) const { return blocks_[block]; }

private:
  std::vector<uint64_t> blocks_;
  uint64_t const size_;
};
} // namespace algo
