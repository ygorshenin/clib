#pragma once

#include <cstddef>
#include <vector>

namespace algo {
class BitVector {
 public:
#define BLOCK_OFFSET(bit)        \
  const size_t block = bit >> 6; \
  const size_t offset = bit & 0x3F

  BitVector(size_t size) : blocks_((size + 63) >> 6), size_(size) {}

  inline void Set(size_t bit) {
    BLOCK_OFFSET(bit);
    blocks_[block] |= static_cast<uint64_t>(1) << offset;
  }

  inline void Clear(size_t bit) {
    BLOCK_OFFSET(bit);
    blocks_[block] &= ~(static_cast<uint64_t>(1) << offset);
  }

  inline bool Test(size_t bit) const {
    BLOCK_OFFSET(bit);
    return blocks_[block] & (static_cast<uint64_t>(1) << offset);
  }

  inline size_t Size() const { return size_; }

  inline size_t NumBlocks() const { return blocks_.size(); }

  inline uint64_t& Block(size_t block) { return blocks_[block]; }

  inline const uint64_t& Block(size_t block) const { return blocks_[block]; }

 private:
  std::vector<uint64_t> blocks_;
  size_t const size_;
};
}  // namespace algo
