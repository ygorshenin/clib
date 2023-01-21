#pragma once

#include <cstdint>

namespace algo {
inline constexpr uint64_t GenL(uint8_t k, uint8_t i) {
  return i >= 64 ? 0 : GenL(k, i + k) | (static_cast<uint64_t>(1) << i);
}

inline constexpr uint64_t L(uint8_t k) { return GenL(k, 0 /* i */); }

inline constexpr uint64_t H(uint8_t k) { return L(k) << (k - 1); }

inline uint64_t BytesPopCount(uint64_t x) noexcept {
  x = x - ((x & H(2)) >> 1);
  x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
  x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F;
  return x;
}

inline uint8_t PopCount(uint64_t x) noexcept { return (BytesPopCount(x) * L(8)) >> 56; }

inline uint8_t CeilLog(uint64_t x) noexcept {
#define CHECK_RSH(x, msb, offset) \
  if (x >> offset) {              \
    x >>= offset;                 \
    msb += offset;                \
  }

  uint8_t msb = 0;
  CHECK_RSH(x, msb, 32);
  CHECK_RSH(x, msb, 16);
  CHECK_RSH(x, msb, 8);
  CHECK_RSH(x, msb, 4);
  CHECK_RSH(x, msb, 2);
  CHECK_RSH(x, msb, 1);
#undef CHECK_RSH

  return msb;
}

inline uint64_t LSB(uint64_t x) noexcept { return x & (~x + 1); }
}  // namespace algo
