#pragma once

#include <cassert>
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

inline uint8_t LoPosUnsafe(uint64_t x) noexcept {
  static_assert(sizeof(long long) == 8);
  assert(x != 0);
  return __builtin_ctzll(x);
}

inline uint8_t HiPosUnsafe(uint64_t x) noexcept {
  static_assert(sizeof(long long) == 8);
  assert(x != 0);
  return 63 - __builtin_clzll(x);
}

inline uint8_t LoPos(uint64_t x) noexcept { return x == 0 ? 64 : LoPosUnsafe(x); }

inline uint8_t HiPos(uint64_t x) noexcept { return x == 0 ? 0 : HiPosUnsafe(x); }

inline uint64_t LSB(uint64_t x) noexcept { return x & (~x + 1); }
}  // namespace algo
