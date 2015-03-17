#include "algo/kmp.h"

namespace algo {

void BuildPrefixFunction(size_t size, const char* pattern, size_t* prefix) {
  if (!size)
    return;
  size_t psl = 0;  // proper suffix length
  prefix[0] = psl;
  for (size_t i = 1; i < size; ++i) {
    while (psl != 0 && pattern[i] != pattern[psl])
      psl = prefix[psl];
    if (pattern[i] == pattern[psl])
      ++psl;
    prefix[i] = psl;
  }
}

}  // namespace algo
