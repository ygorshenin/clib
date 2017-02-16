#include "algo/numeric/fft.h"

#include <cassert>
#include <cmath>
#include <cstddef>

namespace algo
{
void Butterfly(const std::vector<Complex> &from, std::vector<Complex> &to) {
  const size_t n = from.size();
  if (n == 0)
    return;
  assert((n & (n - 1)) == 0);

  to[0] = from[0];

  for (size_t i = 0, j = 1; j < n; ++j) {
    size_t b = n;
    do {
      b >>= 1;
      i ^= b;
    } while ((i & b) == 0);
    to[j] = from[i];
  }
}

void FFT(std::vector<Complex> &xs, bool forward) {
  const double kPI = acos(-1.0);

  const size_t n = xs.size();
  if (n == 0)
    return;
  assert((n & (n - 1)) == 0);

  std::vector<Complex> ys(n);
  Butterfly(xs, ys);

  for (size_t w = 2, h = 1; w <= n; w *= 2, h *= 2) {
    Complex mul = std::exp(Complex(0, 2 * kPI / w));
    if (!forward)
      mul = std::conj(mul);

    for (size_t i = 0; i < n; i += w) {
      Complex cur = 1;
      for (size_t j = i; j < i + h; ++j) {
        auto a = ys[j] + cur * ys[j + h];
        auto b = ys[j] - cur * ys[j + h];
        cur = cur * mul;
        ys[j] = a;
        ys[j + h] = b;
      }
    }
    if (!forward) {
      for (auto &y : ys)
        y *= 0.5;
    }
  }
  xs.swap(ys);
}
}  // namespace algo
