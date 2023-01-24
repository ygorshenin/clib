#include "solvers/words.h"

#include <cctype>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
using namespace algo::solvers;
using namespace std;

const size_t N = 5;
const size_t M = 6;

int main(int argc, char* argv[]) {
  vector<string> nwords;
  vector<string> mwords;

  string word;
  while (cin >> word) {
    for (auto& c : word)
      c = tolower(c);
    if (word.size() == N)
      nwords.push_back(word);
    if (word.size() == M)
      mwords.push_back(word);
  }

  WordRectangle<N, M> words;
  uint64_t total = 0;
  words.Build(nwords.begin(), nwords.end(), mwords.begin(), mwords.end(), [&](const array<size_t, M>& cols) {
    ++total;
    if (total % 100 != 0)
      return;
    cout << "Current num of solutions: " << total << endl;
    for (const auto& col : cols)
      cout << nwords[col] << '\n';
    cout << '\n';
  });
  cout << "Total num of solutions: " << total << endl;
  return 0;
}
