#pragma once

#include <bit>
#include <cassert>
#include <climits>
#include <cstdint>

namespace algo {
inline constexpr uint64_t GenL(uint8_t k, uint8_t i) {
  return i >= 64 ? 0 : GenL(k, i + k) | (static_cast<uint64_t>(1) << i);
}

// Generates bit pattern of ones separated by |k - 1| zeroes, starting with 1 in
// lsb.
inline constexpr uint64_t L(uint8_t k) {
  assert(k != 0);
  return GenL(k, 0 /* i */);
}

// Generates bit pattern of ones separated by |k - 1| zeroes, starting with |k -
// 1| zeros in lsb.
inline constexpr uint64_t H(uint8_t k) {
  assert(k != 0);
  return L(k) << (k - 1);
}

// Calculates number of ones in bit representation of |x|.
inline constexpr uint8_t PopCount(uint64_t x) noexcept { return std::popcount(x); }

// Calculates position of the least significant 1 in |x|. |x| should not be 0.
inline constexpr uint8_t LoPosUnsafe(uint64_t x) noexcept {
  assert(x != 0);
  return std::countr_zero(x);
}

// Calculates position of the most significant 1 in |x|. |x| should not be 0.
inline constexpr uint8_t HiPosUnsafe(uint64_t x) noexcept {
  assert(x != 0);
  return CHAR_BIT * sizeof(x) - 1 - std::countl_zero(x);
}

// Calculates position of the least significant 1 in |x|. When |x| is zero,
// returns total number of bits in |x|, e.g. 64.
inline constexpr uint8_t LoPos(uint64_t x) noexcept { return x == 0 ? CHAR_BIT * sizeof(x) : LoPosUnsafe(x); }

// Calculates position of the most significant 1 in |x|. When |x| is zero, returns 0.
inline constexpr uint8_t HiPos(uint64_t x) noexcept { return x == 0 ? 0 : HiPosUnsafe(x); }

// Returns mask corresponding to lsb 1 in |x|. When |x| is zero, returns zero
// too.
inline constexpr uint64_t LSB(uint64_t x) noexcept { return x & (~x + 1); }
}  // namespace algo
