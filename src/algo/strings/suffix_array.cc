#include "algo/strings/suffix_array.h"

#include <algorithm>
#include <cassert>
#include <limits>

namespace {
bool LEQ(size_t a1, size_t a2, size_t b1, size_t b2) {
  if (a1 != b1)
    return a1 < b1;
  return a2 <= b2;
}

bool LEQ(size_t a1, size_t a2, size_t a3, size_t b1, size_t b2, size_t b3) {
  if (a1 != b1)
    return a1 < b1;
  return LEQ(a2, a3, b2, b3);
}

template <typename Values>
void RadixSort(size_t n,
               const size_t* keys,
               size_t max_value,
               const Values& values,
               size_t* result_keys) {
  std::vector<size_t> count(max_value);
  for (size_t i = 0; i < n; ++i)
    ++count[values[keys[i]]];
  for (size_t i = 1; i < max_value; ++i)
    count[i] += count[i - 1];
  for (size_t i = n - 1; i < n; --i)
    result_keys[--count[values[keys[i]]]] = keys[i];
}

bool InLeftHalf(size_t n0, size_t pos) {
  return pos < n0;
}

size_t RestoreIndex(size_t n0, size_t pos) {
  return InLeftHalf(n0, pos) ? pos * 3 + 1 : (pos - n0) * 3 + 2;
}

struct SkewWrapper {
  SkewWrapper(size_t n, const uint8_t* s) : n_(n), s_(s) {}

  size_t size() const { return n_; }

  size_t operator[](size_t i) const {
    if (i < n_)
      return static_cast<size_t>(s_[i]) + 1;
    assert(i < n_ + 3);
    return 0;
  }

  const size_t n_;
  const uint8_t* const s_;
};

template <typename Container>
struct Slice {
  Slice(const Container& c, size_t n, size_t offset)
      : c_(c), n_(n), offset_(offset) {}

  size_t operator[](size_t i) const { return c_[i + offset_]; }

  const Container& c_;
  const size_t n_;
  const size_t offset_;
};

template <typename Container>
Slice<Container> MakeSlice(const Container& c, size_t offset) {
  return Slice<Container>(c, c.size(), offset);
}

// Builds suffix array over the string s, where for all i < n: 0 < s[i] <= k.
//
// Result is written to the array |SA|, where SA[i] is the offset of
// the i-th ranked suffix.
//
// For implementation simplicity, it's assumed that s[n] = s[n + 1] = s[n + 2] =
// 0.
//
// Idea and implementation was inspired by "Simple Linear Work Suffix
// Array Construction" by Juha K¨arkk¨ainen and Peter Sanders.
template <typename S>
void RawSkew(size_t n, size_t max_value, S const& s, size_t* SA) {
  size_t const kInvalidId = std::numeric_limits<size_t>::max();

  if (n == 0)
    return;

  if (n == 1) {
    SA[0] = 0;
    return;
  }

  // The number of =1 (mod 3) suffixes is the same as the number of =0
  // (mod 3) suffixes.
  const size_t n0 = (n + 2) / 3;  // Number of =0 (mod 3) suffixes.
  const size_t n1 = (n + 1) / 3;  // Number of =1 (mod 3) suffixes.
  const size_t n2 = n / 3;        // Number of =2 (mod 3) suffixes.

  const size_t n02 = n0 + n2;

  const bool fake1 = n0 != n1;
  assert(n1 + fake1 == n0);
  assert(fake1 == (n % 3 == 1));

  // Generate positions of =(1|2) (mod 3) suffixes.
  std::vector<size_t> s12(n02 + 3);
  std::vector<size_t> SA12(n02 + 3);

  // (n0 - n1) is needed in case when n == 0 (mod 3).  We need a fake
  // =1 (mod 3) suffix for proper sorting of =0 (mod 3) suffixes.
  // Therefore we force here that the number of =1 (mod 3) suffixes
  // should be the same as the number of =0 (mod 3) suffixes. That's
  // why we need that s[n] = s[n + 1] = s[n + 2] = 0.
  for (size_t i = 0, j = 0; i < n + fake1; ++i) {
    if (i % 3 != 0)
      s12[j++] = i;
  }

  RadixSort(n02, s12.data(), max_value + 1, MakeSlice(s, 2), SA12.data());
  RadixSort(n02, SA12.data(), max_value + 1, MakeSlice(s, 1), s12.data());
  RadixSort(n02, s12.data(), max_value + 1, s, SA12.data());

  // Generate lexicographic names for all =(1|2) (mod 3) triples.
  size_t name = 0;
  size_t c0 = kInvalidId;
  size_t c1 = kInvalidId;
  size_t c2 = kInvalidId;
  for (size_t i = 0; i < n02; ++i) {
    auto const pos = SA12[i];
    if (s[pos] != c0 || s[pos + 1] != c1 || s[pos + 2] != c2) {
      c0 = s[pos];
      c1 = s[pos + 1];
      c2 = s[pos + 2];
      ++name;
    }

    // Puts all =1 (mod 3) suffixes to the left part of s12, puts all
    // =2 (mod 3) suffixes to the right part.
    if (pos % 3 == 1)
      s12[pos / 3] = name;
    else
      s12[pos / 3 + n0] = name;
  }

  if (name < n02) {
    // When not all triples unique, we need to build a suffix array
    // for them.
    RawSkew(n02, name, s12, SA12.data());
    for (size_t i = 0; i < n02; ++i)
      s12[SA12[i]] = i + 1;
  } else {
    // When all triples are unique, it's easy to build a suffix array.
    for (size_t i = 0; i < n02; ++i)
      SA12[s12[i] - 1] = i;
  }

  // SA12 is the suffix array for the string s12 now, and all symbols
  // in s12 are unique.

  // Need to do a stable sort for all =0 (mod 3) suffixes.
  std::vector<size_t> s0(n0);
  std::vector<size_t> SA0(n0);
  for (size_t i = 0, j = 0; i < n02; ++i) {
    if (SA12[i] < n0)
      s0[j++] = 3 * SA12[i];
  }

  // s0 is pre-sorted now in accordance with their tails (=1 (mod 3)
  // suffixes). For full sorting we need to do a stable sort =0 (mod
  // 3) suffixes in accordance with their first characters.
  RadixSort(n0, s0.data(), max_value + 1, s, SA0.data());

  // SA0 is the suffix array for the string s0 now, and all symbols in
  // s0 are unique.

  // Okay, need to merge sorted =0 (mod 3) suffixes and =(1|2) (mod 3)
  // suffixes.
  size_t i0 = 0;
  size_t i12 = fake1;
  size_t k = 0;
  while (i12 != n02 && i0 != n0) {
    const size_t p0 = SA0[i0];
    const size_t p12 = RestoreIndex(n0, SA12[i12]);
    assert(p12 / 3 < n0);

    if (InLeftHalf(n0, SA12[i12])) {
      if (LEQ(s[p12], s12[SA12[i12] + n0], s[p0], s12[p0 / 3])) {
        // Suffix =(1|2) (mod 3) is smaller.
        SA[k++] = p12;
        ++i12;
      } else {
        SA[k++] = p0;
        ++i0;
      }
    } else {
      if (LEQ(s[p12], s[p12 + 1], s12[SA12[i12] - n0 + 1], s[p0], s[p0 + 1],
              s12[p0 / 3 + n0])) {
        // Suffix =(1|2) (mod 3) is smaller.
        SA[k++] = p12;
        ++i12;
      } else {
        SA[k++] = p0;
        ++i0;
      }
    }
  }
  for (; i12 != n02; ++k, ++i12)
    SA[k] = RestoreIndex(n0, SA12[i12]);
  for (; i0 != n0; ++k, ++i0)
    SA[k] = SA0[i0];
  assert(k == n);
}
}  // namespace

namespace algo {
void Skew(size_t n, const uint8_t* s, size_t* pos) {
  RawSkew(n, 0xFF /* max_value */, SkewWrapper(n, s), pos);
}

void Skew(const std::string& s, std::vector<size_t>& pos) {
  auto const n = s.size();
  pos.assign(n, 0);
  Skew(n, reinterpret_cast<const uint8_t*>(s.data()), pos.data());
}

void ManberMyers(size_t n, const uint8_t* s, size_t* pos) {
  if (n == 0)
    return;

  const size_t max_class =
      std::max(n, static_cast<size_t>(std::numeric_limits<uint8_t>::max()) + 1);

  std::vector<size_t> npos(n);
  std::vector<size_t> classes(n);
  std::vector<size_t> nclasses(n);

  size_t* cur_pos = pos;
  size_t* next_pos = npos.data();

  for (size_t i = 0; i < n; ++i) {
    cur_pos[i] = i;
    classes[cur_pos[i]] = static_cast<size_t>(s[i]);
  }
  RadixSort(n, cur_pos, max_class, classes.data(), next_pos);

  for (size_t h = 1; h < n; h <<= 1) {
    for (size_t i = 0; i < n; ++i) {
      if (next_pos[i] < h)
        next_pos[i] += n - h;
      else
        next_pos[i] -= h;
    }
    RadixSort(n, next_pos, max_class, classes.data(), cur_pos);

    size_t cur_class = 0;
    nclasses[cur_pos[0]] = cur_class;
    for (size_t i = 1; i < n; ++i) {
      const size_t cp = cur_pos[i];
      const size_t pp = cur_pos[i - 1];

      cur_class += classes[cp] != classes[pp] ||
                   classes[(cp + h) % n] != classes[(pp + h) % n];
      nclasses[cp] = cur_class;
    }

    std::swap(cur_pos, next_pos);
    nclasses.swap(classes);
  }

  if (next_pos != pos)
    std::copy(next_pos, next_pos + n, pos);
}

void Kasai(size_t n,
           const uint8_t* s,
           const size_t* pos,
           size_t* rank,
           size_t* lcp) {
  for (size_t i = 0; i < n; ++i)
    rank[pos[i]] = i;

  size_t h = 0;
  for (size_t i = 0; i < n; ++i) {
    if (rank[i] == 0)
      continue;
    size_t j = pos[rank[i] - 1];
    while (s[i + h] == s[j + h])
      ++h;
    lcp[rank[i] - 1] = h;
    if (h)
      --h;
  }
}
}  // namespace algo
