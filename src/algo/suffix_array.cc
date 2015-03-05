#include "algo/suffix_array.h"

#include <algorithm>
#include <limits>
#include <vector>

namespace {

void RadixSort(int n,
               const int* keys,
               int max_value,
               const int* values,
               int result_keys[]) {
  std::vector<int> count(max_value + 1);
  for (int i = 0; i < n; ++i)
    ++count[values[keys[i]]];
  for (int i = 1; i <= max_value; ++i)
    count[i] += count[i - 1];
  for (int i = n - 1; i >= 0; --i)
    result_keys[--count[values[keys[i]]]] = keys[i];
}

}  // namespace

namespace algo {

void SortCyclicShifts(int n, const char* s, int* pos) {
  if (!n)
    return;

  const int max_char = static_cast<int>(std::numeric_limits<char>::max());
  const int max_class = std::max(max_char, n);

  std::vector<int> npos(n);
  std::vector<int> classes(n);
  std::vector<int> nclasses(n);

  int* cur_pos = pos;
  int* next_pos = npos.data();

  for (int i = 0; i < n; ++i) {
    cur_pos[i] = i;
    classes[cur_pos[i]] = static_cast<int>(s[i]);
  }
  RadixSort(n, cur_pos, max_class, classes.data(), next_pos);

  for (int p = 1; (1 << p) < n; ++p) {
    const int h = 1 << (p - 1);
    for (int i = 0; i < n; ++i) {
      next_pos[i] -= h;
      if (next_pos[i] < 0)
        next_pos[i] += n;
    }
    RadixSort(n, next_pos, max_class, classes.data(), cur_pos);

    nclasses[cur_pos[0]] = 0;
    for (int i = 1; i < n; ++i) {
      if (classes[cur_pos[i]] == classes[cur_pos[i - 1]] &&
          classes[(cur_pos[i] + h) % n] == classes[(cur_pos[i - 1] + h) % n]) {
        nclasses[cur_pos[i]] = nclasses[cur_pos[i - 1]];
      } else {
        nclasses[cur_pos[i]] = nclasses[cur_pos[i - 1]] + 1;
      }
    }

    std::swap(cur_pos, next_pos);
    nclasses.swap(classes);
  }

  if (next_pos != pos)
    std::copy(next_pos, next_pos + n, pos);
}

}  // namespace algo
