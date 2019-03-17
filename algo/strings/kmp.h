#pragma once

#include <cstddef>
#include <vector>

namespace algo {
// Builds Knuth-Morris-Pratt prefix function on a string |pattern| of
// size |size|. |prefix| must be an array of size at least |size|, and
// after function call prefix[i] will contain a length of a longest
// proper suffix of pattern[0 .. i+1) which is also a prefix of
// pattern[0 .. i).
void BuildPrefixFunction(size_t size, const char* pattern, size_t* prefix);

// Finds all occurences of |pattern| in |text| and stores them to
// |matches|. Note that |matches| won't be cleared at the function
// entry.
void KMP(size_t text_size, const char* text, size_t pattern_size, const char* pattern, std::vector<size_t>& matches);

// Computes a z-function on |text| of size |size|.  For all i except 0
// z[i] is a maximum value such that text[i .. i+z[i]) == text[0 .. z[i]).
void Z(size_t size, const char* text, size_t* z);
}  // namespace algo
