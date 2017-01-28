#include "algo/bits/bits.h"

#include <cassert>

namespace algo {
uint8_t PopCount(uint64_t x) noexcept {
  x = x - ((x & 0xAAAAAAAAAAAAAAAA) >> 1);
  x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
  x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F;
  return (x * 0x0101010101010101) >> 56;
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
} // namespace algo
