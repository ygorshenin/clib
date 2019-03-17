#pragma once

#include <string>
#include <vector>

#include <cstddef>
#include <cstdint>

namespace algo {
// Builds suffix array for |s| and stores their indices in a |pos|
// array. Size of |pos| array should be equal to |n|.
//
// Time complexity: O(n)
void Skew(size_t n, const uint8_t* s, size_t* pos);
void Skew(const std::string& s, std::vector<size_t>& pos);

// Sorts cyclic shifts of |s| and stores their indices in a |pos|
// array. Size of |pos| array should be equal to |n|.
//
// Time complexity: O(n * log(n))
void ManberMyers(size_t n, const uint8_t* s, size_t* pos);

// Finds LCP between each pair of adjacent sorted suffixes of sequence
// |s| and stores them in an |lcp| array. So, lcp[0] = LCP(s[pos[0]
// .. n), s[pos[1] .. n)), lcp[1] = LCP(s[pos[1] .. n), s[pos[2]
// .. n)), ... As a side effect, function stores ranks of suffixes in
// an array |rank|.
//
// Time complexity: O(n)
void Kasai(size_t n, const uint8_t* s, const size_t* pos, size_t* rank, size_t* lcp);
}  // namespace algo
