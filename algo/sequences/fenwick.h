#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

namespace algo {
template <typename T>
class Fenwick {
public:
  explicit Fenwick(size_t size) : m_buffer(size) {}

  // Adds |value| to an element on a position |index|.
  void Add(size_t index, const T& value) {
    assert(index < m_buffer.size());

    while (index < m_buffer.size()) {
      m_buffer[index] += value;
      index = G(index);
    }
  }

  // Returns sum on a range [0, |to|).
  T Sum(size_t to) const {
    assert(to <= m_buffer.size());

    T result{};
    while (to != 0) {
      --to;
      result += m_buffer[to];
      to = F(to);
    }
    return result;
  }

  // Returns sum on a range [|from|, |to|).
  T Sum(size_t from, size_t to) const {
    assert(from <= to);
    if (from == to)
      return T{};
    return Sum(to) - Sum(from);
  }

  size_t Size() const { return m_buffer.size(); }

private:
  static size_t F(size_t index) { return index & (index + 1); }
  static size_t G(size_t index) { return index | (index + 1); }

  std::vector<T> m_buffer;
};
}  // namespace algo
