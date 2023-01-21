#include "solvers/nqueens.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace algo::solvers;

int main() {
  int n;
  cin >> n;

  uint64_t result = 0;
  NQueens{}.Solve(n, [&result](const NQueens::Candidates& /* candidates */) { ++result; });

  cout << result << endl;
  return 0;
}
