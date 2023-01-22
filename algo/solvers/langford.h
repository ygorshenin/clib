#pragma once

#include <cassert>
#include <vector>

namespace algo::solvers {
class Langford {
public:
  template <typename Fn>
  void SolveNaive(int n, Fn&& fn) const {
    std::vector<int> ps(n + 1);
    for (int i = 0; i < n; ++i)
      ps[i] = i + 1;
    assert(ps[n] == 0);

    std::vector<int> xs(2 * n);

    return NaiveRec(/* level= */ 0, n, ps, xs, fn);
  }

  template <typename Fn>
  void Solve(int n, Fn&& fn) const {
    std::vector<int> ps(n + 1);
    for (int i = 0; i < n; ++i)
      ps[i] = i + 1;
    assert(ps[n] == 0);

    std::vector<int> xs(2 * n);
    std::vector<int> js(2 * n);

    std::vector<int> ls(2 * n);
    int lstop = 0;

    int level = 0;
    int j = 0;
    int k = 0;

  enter:
    if (ps[0] == 0) {
      fn(xs);
      goto downgrade;
    }
    while (xs[level] != 0)
      ++level;

    j = 0;
    k = ps[j];

  loop : {
    if (level + k + 1 >= 2 * n) {
      xs[level] = 0;
      goto downgrade;
    }

    if (xs[level + k + 1] == 0) {
      xs[level] = k;
      xs[level + k + 1] = -k;
      js[level] = j;
      ps[j] = ps[k];
      ls[lstop++] = level++;
      goto enter;
    }
  }

  next:
    j = ps[j];
    k = ps[k];
    if (k != 0)
      goto loop;

    xs[level] = 0;

  downgrade:
    if (level == 0)
      goto finish;

    level = ls[--lstop];
    j = js[level];
    k = xs[level];
    xs[level + k + 1] = 0;
    ps[j] = k;
    goto next;

  finish:
    return;
  }

private:
  template <typename Fn>
  void NaiveRec(int level, int n, std::vector<int>& ps, std::vector<int>& xs, Fn&& fn) const {
    if (ps[0] == 0) {
      fn(xs);
      return;
    }

    while (xs[level] != 0)
      ++level;

    for (int j = 0; ps[j] != 0; j = ps[j]) {
      const int k = ps[j];

      if (level + k + 1 >= 2 * n)
        break;
      if (xs[level + k + 1] != 0)
        continue;

      xs[level] = k;
      xs[level + k + 1] = -k;
      ps[j] = ps[k];

      NaiveRec(level + 1, n, ps, xs, fn);

      xs[level + k + 1] = 0;
      ps[j] = k;
    }
    xs[level] = 0;
  }
};  // namespace algo::solvers
}  // namespace algo::solvers
