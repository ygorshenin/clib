#include <gtest/gtest.h>

#include "numeric/matrix.h"

namespace algo {

TEST(Matrix, Basic) {
  const size_t kHeight = 3;
  const size_t kWidth = 4;
  int data[kHeight * kWidth] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  MatrixAdapter<int> matrix(kHeight, kWidth, data);
  ASSERT_EQ(kHeight, matrix.Height());
  ASSERT_EQ(kWidth, matrix.Width());
  ASSERT_EQ(kHeight * kWidth, matrix.Size());

  int counter = 0;
  for (size_t row = 0; row < matrix.Height(); ++row) {
    for (size_t col = 0; col < matrix.Width(); ++col) {
      ASSERT_EQ(counter, matrix(row, col));
      ++counter;
    }
  }
}

TEST(Matrix, Transpose) {
  const size_t kHeight = 3;
  const size_t kWidth = 4;
  int data[kHeight * kWidth] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  int tdata[kHeight * kWidth];

  MatrixAdapter<int> a(kHeight, kWidth, data);
  MatrixAdapter<int> b(kWidth, kHeight, tdata);
  MatrixTranspose<int, MatrixAdapter> transpose(a, b);

  transpose.Go();

  for (size_t row = 0; row < b.Height(); ++row) {
    for (size_t col = 0; col < b.Width(); ++col)
      ASSERT_EQ(a(col, row), b(row, col));
  }
}

}  // namespace algo
