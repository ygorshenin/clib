#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

namespace algo {
template <typename T>
const T& GetKth(const T* a, size_t lena, const T* b, size_t lenb, size_t k) {
  assert(std::is_sorted(a, a + lena));
  assert(std::is_sorted(b, b + lenb));
  assert(k < lena + lenb);

  if (lena == 0)
    return b[k];
  if (lenb == 0)
    return a[k];

  const auto n = lena / 2;
  const auto m = lenb / 2;
  const auto h = n + m + 1;

  assert(a[n] != b[m]);

  if (a[n] > b[m])
    return GetKth(b, lenb, a, lena, k);
  if (k < h)
    return GetKth(a, lena, b, m, k);
  return GetKth(a + n + 1, lena - n - 1, b, lenb, k - n - 1);
}

template <typename T>
const T& GetKth(const std::vector<T>& a, const std::vector<T>& b, size_t k) {
  return GetKth(a.data(), a.size(), b.data(), b.size(), k);
}
}  // namespace algo
