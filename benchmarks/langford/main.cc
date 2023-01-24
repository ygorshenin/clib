#include <solvers/langford.h>

#include <gflags/gflags.h>

#include <iostream>
#include <sstream>

using namespace algo::solvers;
using namespace std;

DEFINE_bool(use_naive_solver, false, "When true, naive solver will be used");

int main(int argc, char* argv[]) {
  {
    ostringstream os;
    os << "Usage: echo n | " << argv[0] << " [flags]";
    gflags::SetUsageMessage(os.str());
  }
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  int n;
  cin >> n;

  uint64_t count = 0;
  if (FLAGS_use_naive_solver)
    Langford{}.SolveNaive(n, [&count](const vector<int>& /* xs */) { ++count; });
  else
    Langford{}.Solve(n, [&count](const vector<int>& /* xs */) { ++count; });
  cout << count << endl;
  return 0;
}
