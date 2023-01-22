#include <solvers/langford.h>

#include <iostream>

using namespace algo::solvers;
using namespace std;

int main() {
  int n;
  cin >> n;

  uint64_t count = 0;
  Langford{}.Solve(n, [&count](const vector<int>& /* xs */) { ++count; });
  cout << count << endl;
  return 0;
}
