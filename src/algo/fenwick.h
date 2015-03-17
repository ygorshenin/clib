#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

template <typename T>
class Fenwick {
 public:
  Fenwick(size_t size) : buffer_(size) {}

  // Adds |value| to an element on a position |index|.
  void Add(size_t index, const T& value) {
    assert(index < buffer_.size());

    while (index < buffer_.size()) {
      buffer_[index] += value;
      index = G(index);
    }
  }

  // Returns sum on a range [0, |to|).
  T Sum(size_t to) const {
    assert(to <= buffer_.size());

    T result{};
    while (to != 0) {
      --to;
      result += buffer_[to];
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

  inline size_t size() const { return buffer_.size(); }

 private:
  static inline size_t F(size_t index) { return index & (index + 1); }
  static inline size_t G(size_t index) { return index | (index + 1); }

  std::vector<T> buffer_;
};
