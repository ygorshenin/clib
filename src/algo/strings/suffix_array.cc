#include "algo/strings/suffix_array.h"

#include <algorithm>
#include <limits>
#include <vector>

namespace {

void RadixSort(size_t n,
               const size_t* keys,
               size_t max_value,
               const size_t* values,
               size_t* result_keys) {
  std::vector<size_t> count(max_value + 1);
  for (size_t i = 0; i < n; ++i)
    ++count[values[keys[i]]];
  for (size_t i = 1; i <= max_value; ++i)
    count[i] += count[i - 1];
  for (size_t i = 0, j = n - 1; i < n; ++i, --j)
    result_keys[--count[values[keys[j]]]] = keys[j];
}

}  // namespace

namespace algo {

void ManberMyers(size_t n, const uint8_t* s, size_t* pos) {
  if (!n)
    return;

  const size_t max_class =
      std::max(n, static_cast<size_t>(std::numeric_limits<uint8_t>::max()));

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

  for (int p = 1; (static_cast<size_t>(1) << p) < n; ++p) {
    const size_t h = static_cast<size_t>(1) << (p - 1);
    for (size_t i = 0; i < n; ++i) {
      if (next_pos[i] < h)
        next_pos[i] += n - h;
      else
        next_pos[i] -= h;
    }
    RadixSort(n, next_pos, max_class, classes.data(), cur_pos);

    nclasses[cur_pos[0]] = 0;
    for (size_t i = 1; i < n; ++i) {
      const size_t cp = cur_pos[i];
      const size_t pp = cur_pos[i - 1];

      if (classes[cp] == classes[pp] &&
          classes[(cp + h) % n] == classes[(pp + h) % n]) {
        nclasses[cp] = nclasses[pp];
      } else {
        nclasses[cp] = nclasses[pp] + 1;
      }
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
