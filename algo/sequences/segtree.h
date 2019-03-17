#pragma once

#include <cassert>
#include <cstddef>

namespace algo {

template <typename Monoid>
Monoid Mappend(const Monoid& lhs, const Monoid& rhs);

template <typename Monoid>
class SegTree {
 public:
  SegTree(size_t size) : data_(2 * Ceil2Pow(size)), size_(size) {}

  size_t size() const { return size_; }

  void Set(size_t index, const Monoid& m) {
    assert(index < size());
    SetImpl(
        0 /* pos */, 0 /* left_bound */, size() /* right_bound */, index, m);
  }

  const Monoid& Get(size_t index) const {
    assert(index < size());
    return GetImpl(
        0 /* pos */, 0 /* left_bound */, size() /* right_bound */, index);
  }

  Monoid Get(size_t from, size_t to) const {
    assert(from <= to);
    assert(to <= size());
    if (from == to)
      return Monoid();
    return GetImpl(
        0 /* pos */, 0 /* left_bound */, size() /* right_bound */, from, to);
  }

 private:
  void SetImpl(size_t pos,
               size_t left_bound,
               size_t right_bound,
               size_t index,
               const Monoid& m);
  const Monoid& GetImpl(size_t pos,
                        size_t left_bound,
                        size_t right_bound,
                        size_t index) const;
  Monoid GetImpl(size_t pos,
                 size_t left_bound,
                 size_t right_bound,
                 size_t from,
                 size_t to) const;

  static size_t Ceil2Pow(size_t n) {
    size_t p = 1;
    while (p < n)
      p <<= 1;
    return p;
  }

  static size_t LeftChild(size_t pos) { return 2 * pos + 1; }
  static size_t RightChild(size_t pos) { return 2 * pos + 2; }

  std::vector<Monoid> data_;
  size_t size_;
};

template <typename Monoid>
void SegTree<Monoid>::SetImpl(size_t pos,
                              size_t left_bound,
                              size_t right_bound,
                              size_t index,
                              const Monoid& m) {
  assert(left_bound < right_bound);
  if (left_bound + 1 == right_bound) {
    assert(left_bound == index);
    data_[pos] = m;
    return;
  }
  size_t middle = left_bound + (right_bound - left_bound) / 2;
  size_t left_child_pos = LeftChild(pos);
  size_t right_child_pos = RightChild(pos);
  if (index < middle)
    SetImpl(left_child_pos, left_bound, middle, index, m);
  else
    SetImpl(right_child_pos, middle, right_bound, index, m);
  data_[pos] = Mappend(data_[left_child_pos], data_[right_child_pos]);
}

template <typename Monoid>
const Monoid& SegTree<Monoid>::GetImpl(size_t pos,
                                       size_t left_bound,
                                       size_t right_bound,
                                       size_t index) const {
  assert(left_bound < right_bound);
  if (left_bound + 1 == right_bound) {
    assert(index == left_bound);
    return data_[pos];
  }
  size_t middle = left_bound + (right_bound - left_bound) / 2;
  if (index < middle)
    return GetImpl(LeftChild(pos), left_bound, middle, index);
  return GetImpl(RightChild(pos), middle, right_bound, index);
}

template <typename Monoid>
Monoid SegTree<Monoid>::GetImpl(size_t pos,
                                size_t left_bound,
                                size_t right_bound,
                                size_t from,
                                size_t to) const {
  assert(left_bound < right_bound);
  if (from >= right_bound || to <= left_bound)
    return Monoid();
  if (from <= left_bound && to >= right_bound)
    return data_[pos];
  size_t middle = left_bound + (right_bound - left_bound) / 2;
  size_t left_child_pos = LeftChild(pos);
  size_t right_child_pos = RightChild(pos);
  Monoid a = GetImpl(left_child_pos, left_bound, middle, from, to);
  Monoid b = GetImpl(right_child_pos, middle, right_bound, from, to);
  return Mappend(a, b);
}

}  // namespace algo
