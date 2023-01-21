#pragma once

#include <bits/bits.h>

#include <iostream>

#include <cassert>
#include <cstdint>
#include <vector>

namespace algo::solvers {
class NQueens {
public:
  template <typename Fn>
  void Solve(uint8_t n, Fn&& fn) const {
    assert(2 * n - 1 <= 64);

    std::vector<uint64_t> candidates(n);

    const uint64_t all = (static_cast<uint64_t>(1) << n) - 1;

    // a are bits corresponding to p[i]
    // b are bits corresponding to p[i] + (n - 1 - i)
    // c are bits corresponding to p[i] + i
    uint64_t a = 0;
    uint64_t b = 0;
    uint64_t c = 0;

    uint8_t level = 0;

  enter : {
    if (level == n) {
      fn(candidates);
      goto downgrade;
    }
    assert(level < n);
    candidates[level] = all & ~(a | (b >> (n - 1 - level)) | (c >> level));
  }

  loop : {
    const auto candidate = LSB(candidates[level]);
    if (candidate == 0)
      goto downgrade;

    a = a ^ candidate;
    b = b ^ (candidate << (n - 1 - level));
    c = c ^ (candidate << level);
    ++level;
    goto enter;
  }

  downgrade : {
    if (level == 0)
      goto finish;
    --level;

    const auto candidate = LSB(candidates[level]);
    candidates[level] ^= candidate;
    a = a ^ candidate;
    b = b ^ (candidate << (n - 1 - level));
    c = c ^ (candidate << level);
    goto loop;
  }

  finish:
    return;
  }
};
}  // namespace algo::solvers
