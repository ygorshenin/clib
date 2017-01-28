#include "algo/math/math.h"

namespace algo {
bool IsPrime(uint64_t a) {
  if (a < 2)
    return false;
  if (a == 2)
    return true;

  if (a % 2 == 0)
    return false;

  for (uint64_t i = 3; i <= a / i; i += 2) {
    if (a % i == 0)
      return false;
  }

  return true;
}
} // namespace algo
