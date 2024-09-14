#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <optional>
#include <vector>

namespace algo {
template <typename T>
T Ceil2Pow(T n) {
  T p{1};
  while (p < n)
    p *= 2;
  return p;
}

template <typename M>
class SegTree {
public:
  struct Iterator {
    using difference_type = size_t;
    using value_type = M;
    using pointer = const M*;
    using reference = const M&;
    using iterator_category = std::random_access_iterator_tag;

    Iterator(const SegTree& tree, size_t offset) : m_tree{&tree}, m_offset{offset} {}

    auto operator*() const { return m_tree->Get(m_offset); }

    bool operator==(const Iterator& rhs) const { return m_offset == rhs.m_offset; }

    bool operator!=(const Iterator& rhs) const { return !(*this == rhs); }

    Iterator& operator+=(size_t n) {
      m_offset += n;
      return *this;
    }

    Iterator& operator++() {
      ++m_offset;
      return *this;
    }

    Iterator operator++(int) {
      auto it = *this;
      ++*this;
      return it;
    }

    Iterator& operator--() {
      --m_offset;
      return *this;
    }

    Iterator operator--(int) {
      auto it = *this;
      --*this;
      return it;
    }

    friend size_t operator-(const Iterator& lhs, const Iterator& rhs) { return lhs.m_offset - rhs.m_offset; }

    friend Iterator operator+(Iterator it, size_t offset) {
      it += offset;
      return it;
    }

    const SegTree* m_tree;
    size_t m_offset{};
  };

  explicit SegTree(size_t n) : m_n{n}, m_nodes{2 * Ceil2Pow(n)} {}

  Iterator begin() const { return Iterator{*this, /* offset= */ 0}; }
  Iterator end() const { return Iterator{*this, /* offset= */ m_n}; }

  void Color(size_t from, size_t to, const M& m) {
    return Color(/* curr= */ 1, /* left= */ 0, /* right= */ m_n, from, to, m);
  }
  void Color(const Iterator& begin, const Iterator& end, const M& m) { return Color(begin.m_offset, end.m_offset, m); }
  void Set(size_t i, const M& m) { return Color(/* from= */ i, /* to= */ i + 1, m); }

  M Sum(size_t from, size_t to) const { return Sum(/* curr= */ 1, /* left= */ 0, /* right= */ m_n, from, to); }
  M Sum(const Iterator& begin, const Iterator& end) const { return Sum(begin.m_offset, end.m_offset); }
  M Get(size_t i) const { return Sum(/* from= */ i, /* to= */ i + 1); }

  size_t Size() const { return m_n; }

private:
  struct Node {
    std::optional<M> m_color;
    M m_sum{};
  };

  void Color(size_t curr, size_t left, size_t right, size_t from, size_t to, const M& m) {
    from = std::max(from, left);
    to = std::min(to, right);

    if (from >= to)
      return;
    if (to <= left || from >= right)
      return;
    if (from == left && to == right) {
      m_nodes[curr].m_color = m;
      m_nodes[curr].m_sum = (right - left) * m;
      return;
    }

    PropagateColor(curr, left, right);

    const auto mi = left + (right - left) / 2;
    Color(2 * curr, left, mi, from, to, m);
    Color(2 * curr + 1, mi, right, from, to, m);
    m_nodes[curr].m_color = {};
    m_nodes[curr].m_sum = m_nodes[2 * curr].m_sum + m_nodes[2 * curr + 1].m_sum;
  }

  M Sum(size_t curr, size_t left, size_t right, size_t from, size_t to) const {
    from = std::max(from, left);
    to = std::min(to, right);

    if (from >= to)
      return {};
    if (to <= left || from >= right)
      return {};
    if (from == left && to == right)
      return m_nodes[curr].m_sum;
    if (m_nodes[curr].m_color.has_value()) {
      return *m_nodes[curr].m_color * (to - from);
    }

    const auto mi = left + (right - left) / 2;
    return Sum(2 * curr, left, mi, from, to) + Sum(2 * curr + 1, mi, right, from, to);
  }

  void PropagateColor(size_t curr, size_t left, size_t right) {
    const auto& color = m_nodes[curr].m_color;
    if (!color.has_value())
      return;
    const auto mi = left + (right - left) / 2;
    Color(2 * curr, left, mi, left, right, *color);
    Color(2 * curr + 1, mi, right, left, right, *color);
  }

  size_t m_n{};
  std::vector<Node> m_nodes;
};

template <typename M>
struct FastSegTree {
  explicit FastSegTree(size_t n) : m_base{Ceil2Pow(n)}, m_n{n}, m_values(2 * m_base) {}

  static size_t Parent(size_t i) { return i >> 1; }
  static size_t Left(size_t i) { return 2 * i; }
  static size_t Right(size_t i) { return 2 * i + 1; }

  void Set(size_t i, const M& m) {
    i += m_base;
    m_values[i] = m;

    for (i = Parent(i); i != 0; i = Parent(i)) {
      m_values[i] = m_values[Left(i)] + m_values[Right(i)];
    }
  }

  M Get(size_t from) const {
    return Sum(from, from + 1);
  }

  M Sum(size_t from, size_t to) const {
    if (from >= to)
      return M{};

    --to;
    from = m_base + from;
    to = m_base + to;

    assert(from <= to);
    if (from == to)
      return m_values[from];

    auto fromPart = m_values[from];
    auto toPart = m_values[to];

    while (true) {
      const auto pfrom = Parent(from);
      const auto pto = Parent(to);

      if (pfrom == pto)
        break;

      if (from == Left(pfrom))
        fromPart = fromPart + m_values[Right(pfrom)];
      if (to == Right(pto))
        toPart = m_values[Left(pto)] + toPart;

      from = pfrom;
      to = pto;
    }

    return fromPart + toPart;
  }

  size_t Size() const { return m_n; }

  size_t m_base{};
  size_t m_n{};
  std::vector<M> m_values;
};
}  // namespace algo
