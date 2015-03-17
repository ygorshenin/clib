#pragma once

#include <cstddef>

namespace algo {

// Builds Knuth-Morris-Pratt prefix function on a string |pattern| of
// size |size|. |prefix| must be an array of size at least |size|, and
// after function call prefix[i] will contain a length of a longest
// proper suffix of pattern[0 .. i + 1) which is also a prefix of
// pattern[0 .. i).
void BuildPrefixFunction(size_t size, const char* pattern, size_t* prefix);

}  // namespace algo
