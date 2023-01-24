#include <gtest/gtest.h>

#include "solvers/words.h"

#include <string>
#include <vector>

using namespace algo::solvers;
using namespace std;

namespace algo {
TEST(Solvers, WordsSmoke) {
  const vector<string> words4{"abcd", "efgh", "ijkl"};
  const vector<string> words3{"dhl", "cgk", "bfj", "aei"};

  WordRectangle</* N= */ 3, /* M= */ 4> words;

  uint64_t total = 0;
  words.Build(words3.begin(), words3.end(), words4.begin(), words4.end(),
              [&](const std::array<size_t, 4>& /* cols */) { ++total; });
  ASSERT_EQ(total, 1);
}

TEST(Solvers, WordsSmoke2) {
  const vector<string> words5{"awoke", "slums", "stage", "tepid", "total", "using", "slums"};
  const vector<string> words6{"lowest", "making", "sledge", "status", "utopia"};

  WordRectangle</* N= */ 5, /* M= */ 6> words;

  uint64_t total = 0;
  words.Build(words5.begin(), words5.end(), words6.begin(), words6.end(),
              [&](const std::array<size_t, 6>& /* cols */) { ++total; });
  ASSERT_EQ(total, 2);
}
}  // namespace algo
