#include "algo/kmp.h"

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

void KMP(size_t text_size,
         const char* text,
         size_t pattern_size,
         const char* pattern,
         std::vector<size_t>& matches) {
  if (pattern_size == 0) {
    for (size_t i = 0; i <= text_size; ++i)
      matches.push_back(i);
    return;
  }

  std::vector<size_t> prefix(pattern_size);
  BuildPrefixFunction(pattern_size, pattern, prefix.data());

  size_t psl = 0;
  for (size_t i = 0; i < text_size; ++i) {
    while (psl != 0 && text[i] != pattern[psl])
      psl = prefix[psl - 1];
    if (text[i] == pattern[psl])
      ++psl;
    if (psl == pattern_size) {
      matches.push_back(i + 1 - pattern_size);
      psl = prefix[psl - 1];
    }
  }
}

}  // namespace algo
