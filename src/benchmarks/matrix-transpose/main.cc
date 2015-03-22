#include <cstdio>

#include "algo/numeric/matrix.h"

const size_t kHeight = static_cast<size_t>(1) << 14;
const size_t kWidth = static_cast<size_t>(1) << 14;
const size_t kSize = kHeight * kWidth;

int main() {
  size_t* ma = new size_t[kSize];
  size_t* mb = new size_t[kSize];

  for (size_t i = 0; i < kSize; ++i)
    ma[i] = i;

  algo::MatrixAdapter<size_t> a(kHeight, kWidth, ma);
  algo::MatrixAdapter<size_t> b(kWidth, kHeight, mb);
  algo::TransposeFast(a, b);

  for (size_t i = 0; i < b.width(); ++i) {
    if (b(0, i) != i * kWidth) {
      fprintf(stderr, "Matrix transposition error\n");
      return 1;
    }
  }

  delete [] mb;
  delete [] ma;
  return 0;
}
