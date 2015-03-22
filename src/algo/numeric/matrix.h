#pragma once

#include <cassert>
#include <cstddef>

#define CLS 64  // cache-line size

namespace algo {

namespace impl {

template <typename T, template <typename> class Matrix>
void TransposeImpl(size_t min_row,
                   size_t max_row,
                   size_t min_col,
                   size_t max_col,
                   const Matrix<T>& a,
                   Matrix<T>& b);

}  // namespace impl

// Adapter from 1d array to 2d matrix.
template <typename T>
class MatrixAdapter {
 public:
  MatrixAdapter(size_t height, size_t width, T* buffer)
      : height_(height), width_(width), buffer_(buffer) {}

  inline size_t height() const { return height_; }

  inline size_t width() const { return width_; }

  inline size_t size() const { return height_ * width_; }

  T& operator()(size_t row, size_t col) { return buffer_[row * width_ + col]; }

  const T& operator()(size_t row, size_t col) const {
    return buffer_[row * width_ + col];
  }

 private:
  size_t height_;
  size_t width_;
  T* buffer_;
};

template <typename T, template <typename> class Matrix>
void TransposeFast(const Matrix<T>& a, Matrix<T>& b) {
  assert(a.height() == b.width());
  assert(a.width() == b.height());

  impl::TransposeImpl(0, a.height(), 0, a.width(), a, b);
}

template <typename T, template <typename> class Matrix>
void TransposeSlow(const Matrix<T>& a, Matrix<T>& b) {
  assert(a.height() == b.width());
  assert(a.width() == b.height());

  for (size_t row = 0; row < a.height(); ++row) {
    for (size_t col = 0; col < a.width(); ++col)
      b(col, row) = a(row, col);
  }
}


namespace impl {

template <typename T, template <typename> class Matrix>
void TransposeImpl(size_t min_row,
                   size_t max_row,
                   size_t min_col,
                   size_t max_col,
                   const Matrix<T>& a,
                   Matrix<T>& b) {
  const size_t height = max_row - min_row;
  const size_t width = max_col - min_col;

  if (!height || !width)
    return;

  if (height * sizeof(T) <= CLS && width * sizeof(T) <= CLS) {
    for (size_t row = min_row; row < max_row; ++row) {
      for (size_t col = min_col; col < max_col; ++col)
        b(col, row) = a(row, col);
    }
    return;
  }

  const size_t middle_row = min_row + height / 2;
  const size_t middle_col = min_col + width / 2;
  TransposeImpl(min_row, middle_row, min_col, middle_col, a, b);
  TransposeImpl(min_row, middle_row, middle_col, max_col, a, b);
  TransposeImpl(middle_row, max_row, min_col, middle_col, a, b);
  TransposeImpl(middle_row, max_row, middle_col, max_col, a, b);
}

}  // namespace impl

}  // namespace algo
