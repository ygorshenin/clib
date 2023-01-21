#include "solvers/nqueens.h"

#include <iostream>
#include <vector>

using namespace std;

int main() {
  int n;
  cin >> n;

  uint64_t result = 0;
  algo::solvers::NQueens{}.Solve(n, [&result](const vector<uint64_t>& /* positions */) { ++result; });

  cout << result << endl;
  return 0;
}
