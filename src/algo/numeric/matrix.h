#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>

namespace algo {

// Adapter from 1d array to 2d matrix.
template <typename T>
class MatrixAdapter {
 public:
  MatrixAdapter(size_t height, size_t width, T* buffer)
      : height_(height),
        width_(width),
        size_(height_ * width_),
        buffer_(buffer) {}

  inline size_t height() const { return height_; }

  inline size_t width() const { return width_; }

  inline size_t size() const { return size_; }

  T& operator()(size_t row, size_t col) { return buffer_[row * width_ + col]; }

  const T& operator()(size_t row, size_t col) const {
    return buffer_[row * width_ + col];
  }

 private:
  const size_t height_;
  const size_t width_;
  const size_t size_;
  T* const buffer_;
};

template <typename T, template <typename> class Matrix>
void Transpose(const Matrix<T>& a, Matrix<T>& b) {
  assert(a.height() == b.width());
  assert(a.width() == b.height());

  for (size_t row = 0; row < a.height(); ++row) {
    for (size_t col = 0; col < a.width(); ++col)
      b(col, row) = a(row, col);
  }
}

template <typename T, template <typename> class Matrix>
class MatrixTranspose {
 public:
  static const int kCLS = 64;  // cache-line size estimation

  MatrixTranspose(const Matrix<T>& a, Matrix<T>& b) : a_(a), b_(b) {
    assert(a_.height() == b_.width());
    assert(a_.width() == b_.height());
  }

  void Go() const { Go(0, a_.height(), 0, a_.width()); }

 private:
  void Go(size_t min_row,
          size_t max_row,
          size_t min_col,
          size_t max_col) const {
    static const size_t kLimit =
        std::max(kCLS / sizeof(T), static_cast<size_t>(1));

    const size_t height = max_row - min_row;
    const size_t width = max_col - min_col;

    if (!height || !width)
      return;

    if (height <= kLimit && width <= kLimit) {
      for (size_t row = min_row; row < max_row; ++row) {
        for (size_t col = min_col; col < max_col; ++col)
          b_(col, row) = a_(row, col);
      }
      return;
    }

    const size_t middle_row = min_row + height / 2;
    const size_t middle_col = min_col + width / 2;
    Go(min_row, middle_row, min_col, middle_col);
    Go(min_row, middle_row, middle_col, max_col);
    Go(middle_row, max_row, min_col, middle_col);
    Go(middle_row, max_row, middle_col, max_col);
  }

  const Matrix<T>& a_;
  Matrix<T>& b_;
};

}  // namespace algo
