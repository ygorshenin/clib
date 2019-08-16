#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

namespace algo {

// Adapter from 1d array to 2d matrix.
template <typename T>
class MatrixAdapter {
public:
  MatrixAdapter(size_t height, size_t width, T* buffer)
      : m_height(height), m_width(width), m_size(m_height * m_width), m_buffer(buffer) {}

  size_t Height() const { return m_height; }
  size_t Width() const { return m_width; }
  size_t Size() const { return m_size; }

  T& operator()(size_t row, size_t col) {
    assert(row < m_height);
    assert(col < m_width);
    return m_buffer[row * m_width + col];
  }

  const T& operator()(size_t row, size_t col) const {
    assert(row < m_height);
    assert(col < m_width);
    return m_buffer[row * m_width + col];
  }

private:
  const size_t m_height;
  const size_t m_width;
  const size_t m_size;
  T* const m_buffer;
};

template <typename T>
class Matrix {
public:
  Matrix(size_t height, size_t width) : m_buffer(m_height * m_width), m_height(height), m_width(width) {}

  size_t Height() const { return m_height; }
  size_t Width() const { return m_width; }

  T& operator()(size_t row, size_t col) {
    assert(row < m_height);
    assert(col < m_width);
    return m_buffer[row * m_width + col];
  }

  const T& operator()(size_t row, size_t col) const {
    assert(row < m_height);
    assert(col < m_width);
    return m_buffer[row * m_width + col];
  }

private:
  std::vector<T> m_buffer;
  const size_t m_height;
  const size_t m_width;
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

  MatrixTranspose(const Matrix<T>& a, Matrix<T>& b) : m_a(a), m_b(b) {
    assert(m_a.Height() == m_b.Width());
    assert(m_a.Width() == m_b.Height());
  }

  void Go() const { Go(0, m_a.Height(), 0, m_a.Width()); }

private:
  void Go(size_t minRow, size_t maxRow, size_t minCol, size_t maxCol) const {
    static const size_t kLimit = std::max(kCLS / sizeof(T), static_cast<size_t>(1));

    const size_t height = maxRow - minRow;
    const size_t width = maxCol - minCol;

    if (!height || !width)
      return;

    if (height <= kLimit && width <= kLimit) {
      for (size_t row = minRow; row < maxRow; ++row) {
        for (size_t col = minCol; col < maxCol; ++col)
          m_b(col, row) = m_a(row, col);
      }
      return;
    }

    const size_t middleRow = minRow + height / 2;
    const size_t middleCol = minCol + width / 2;
    Go(minRow, middleRow, minCol, middleCol);
    Go(minRow, middleRow, middleCol, maxCol);
    Go(middleRow, maxRow, minCol, middleCol);
    Go(middleRow, maxRow, middleCol, maxCol);
  }

  const Matrix<T>& m_a;
  Matrix<T>& m_b;
};

}  // namespace algo
