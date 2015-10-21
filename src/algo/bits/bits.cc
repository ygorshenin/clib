#include "algo/bits/bits.h"

namespace algo {
uint8_t PopCount(uint64_t x) noexcept {
  x = (x & 0x5555555555555555) + ((x & 0xAAAAAAAAAAAAAAAA) >> 1);
  x = (x & 0x3333333333333333) + ((x & 0xCCCCCCCCCCCCCCCC) >> 2);
  x = (x & 0x0F0F0F0F0F0F0F0F) + ((x & 0xF0F0F0F0F0F0F0F0) >> 4);
  x = (x & 0x00FF00FF00FF00FF) + ((x & 0xFF00FF00FF00FF00) >> 8);
  x = (x & 0x0000FFFF0000FFFF) + ((x & 0xFFFF0000FFFF0000) >> 16);
  x = x + (x >> 32);
  return static_cast<uint8_t>(x);
}

uint8_t CeilLog(uint64_t x) noexcept {
#define CHECK_RSH(x, msb, offset)                                              \
  if (x >> offset) {                                                           \
    x >>= offset;                                                              \
    msb += offset;                                                             \
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
}  // namespace algo
