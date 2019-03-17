#include "strings/kmp.h"

namespace algo {
void BuildPrefixFunction(size_t size, const char* pattern, size_t* prefix) {
  if (!size)
    return;
  size_t psl = 0;  // proper suffix length
  prefix[0] = psl;
  for (size_t i = 1; i < size; ++i) {
    while (psl != 0 && pattern[i] != pattern[psl])
      psl = prefix[psl - 1];
    if (pattern[i] == pattern[psl])
      ++psl;
    prefix[i] = psl;
  }
}

void KMP(size_t text_size, const char* text, size_t pattern_size, const char* pattern, std::vector<size_t>& matches) {
  if (!pattern_size) {
    for (size_t i = 0; i <= text_size; ++i)
      matches.push_back(i);
    return;
  }

  std::vector<size_t> prefix(pattern_size);
  BuildPrefixFunction(pattern_size, pattern, prefix.data());

  size_t mps = 0;  // matched pattern length
  for (size_t i = 0; i < text_size; ++i) {
    while (mps != 0 && text[i] != pattern[mps])
      mps = prefix[mps - 1];
    if (text[i] == pattern[mps])
      ++mps;
    if (mps == pattern_size) {
      matches.push_back(i + 1 - pattern_size);
      mps = prefix[mps - 1];
    }
  }
}

void Z(size_t size, const char* text, size_t* z) {
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
}  // namespace algo
