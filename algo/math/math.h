#pragma once

#include <cstdint>

namespace algo {
inline uint64_t GCD(uint64_t a, uint64_t b) { return b == 0 ? a : GCD(b, a % b); }

inline uint64_t LCM(uint64_t a, uint64_t b) {
  const uint64_t d = GCD(a, b);
  return a / d * b;
}

bool IsPrime(uint64_t a);
}  // namespace algo
