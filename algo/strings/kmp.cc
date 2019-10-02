#include "strings/kmp.h"

namespace algo {
void BuildPrefixFunction(size_t size, const char *pattern, size_t *prefix) {
  prefix[0] = 0;
  if (size != 0)
    prefix[1] = 0;

  for (size_t i = 1; i < size; ++i) {
    auto p = prefix[i];
    while (p != 0 && pattern[p] != pattern[i])
      p = prefix[p];
    if (pattern[p] == pattern[i])
      ++p;
    prefix[i + 1] = p;
  }
}

void KMP(size_t textSize, const char *text, size_t patternSize,
         const char *pattern, std::vector<size_t> &matches) {
  if (!patternSize) {
    for (size_t i = 0; i <= textSize; ++i)
      matches.push_back(i);
    return;
  }

  std::vector<size_t> prefix(patternSize + 1);
  BuildPrefixFunction(patternSize, pattern, prefix.data());

  size_t j = 0;
  for (size_t i = 0; i < textSize; ++i) {
    while (j != 0 && text[i] != pattern[j])
      j = prefix[j];
    if (text[i] == pattern[j])
      ++j;
    if (j == patternSize) {
      matches.push_back(i + 1 - patternSize);
      j = prefix[j];
    }
  }
}

void Z(size_t size, const char *text, size_t *z) {
  if (!size)
    return;

  z[0] = 0;

  size_t s = 0;
  for (size_t i = 1; i < size; ++i) {
    if (s + z[s] <= i) {
      // Computes z[i] as is, because we can't estimate it.
      size_t zi = 0;
      for (; i + zi < size && text[zi] == text[i + zi]; ++zi)
        ;
      z[i] = zi;
      s = i;
    } else {
      // text[s .. s+z[s]) == text[0 .. z[s]) && r > i =>
      // text[i .. s+z[s]) == text[i-s .. z[s]).
      if (z[i - s] < s + z[s] - i) {
        z[i] = z[i - s];
      } else {
        size_t zi = s + z[s] - i;
        for (; i + zi < size && text[zi] == text[i + zi]; ++zi)
          ;
        z[i] = zi;
        s = i;
      }
    }
  }
}
} // namespace algo
