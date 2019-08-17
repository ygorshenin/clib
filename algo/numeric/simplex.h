#pragma once

#include "numeric/matrix.h"

#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

namespace algo {

// Solves linear programming tasks in the following form:
// fs[0] * xs[0] + fs[1] * xs[1] + ... + fs[n - 1] * xs[n - 1] -> max
//
// as[0][0] * xs[0] + as[0][1] * xs[1] + ... + as[0][n - 1] * xs[n - 1] <= bs[0]
// as[1][0] * xs[0] + as[1][1] * xs[1] + ... + as[1][n - 1] * xs[n - 1] <= bs[1]
// ...
// as[m - 1][0] * xs[0] + as[m - 1][1] * xs[1] + ... + as[m - 1][n - 1] * xs[n - 1] <= bs[m - 1]
//
// Where for all i: bs[i] >= 0, xs[i] >= 0.
//
// Inspired by little-known and hard-to-find, but very nice paper by
// D.E.K.: https://www-cs-faculty.stanford.edu/~knuth/programs/lp.w
class Simplex {
public:
  static inline const double kEps = 1e-10;

  template <typename M>
  explicit Simplex(const M& as, const std::vector<double>& bs, const std::vector<double>& fs, bool verbose = false)
      : m_as(as.Height() + 1, as.Width() + as.Height() + 1)
      , m_trial(m_as.Width())
      , m_brow(m_as.Width())
      , m_bcol(m_as.Height())
      , m_verbose(verbose) {
    assert(as.Height() == bs.size());
    assert(as.Width() == fs.size());

    for (size_t i = 0; i < fs.size(); ++i)
      m_as(0, as.Height() + 1 + i) = -fs[i];

    for (size_t i = 0; i < as.Height(); ++i) {
      assert(bs[i] >= 0);

      m_as(i + 1, 0) = bs[i];
      m_as(i + 1, i + 1) = 1;
      for (size_t j = 0; j < as.Width(); ++j)
        m_as(i + 1, as.Height() + 1 + j) = as(i, j);
      m_brow[i + 1] = i + 1;
      m_bcol[i + 1] = i + 1;
    }

    if (verbose) {
      std::cerr << "Initial table:" << std::endl;
      Display();
    }
  }

  // In case of finite solution, fills |target| with target function
  // value, fills |xs| with variables values and returns
  // true. Otherwise, returns false.
  bool Solve(double& target, std::vector<double>& xs) {
    assert(m_as.Height() > 0);

    uint64_t steps = 0;
    while (true) {
      ++steps;

      bool changed = false;

      assert(m_as.Width() > 0);
      for (size_t j = m_as.Width() - 1; j > 0; --j) {
        if (m_as(0, j) >= 0)
          continue;

        const auto i = GetRowToPivot(j);
        if (i == 0) {
          if (m_verbose)
            std::cerr << "The maximum is infinite, proved after " << steps << " step(s)" << std::endl;
          return false;
        }
        Pivot(i, j);
        if (m_verbose) {
          std::cerr << "After pivoting at (" << i << ", " << j << "):" << std::endl;
          Display();
        }
        changed = true;
      }

      if (!changed)
        break;
    }

    target = m_as(0, 0);
    xs.assign(m_as.Width() - m_as.Height(), 0.0);
    for (size_t i = 0; i < xs.size(); ++i) {
      const auto row = m_brow[m_as.Height() + i];
      if (row != 0)
        xs[i] = m_as(row, 0);
    }

    if (m_verbose) {
      std::cerr << "Total steps: " << steps << std::endl;
      std::cerr << "Target function value: " << target << std::endl;
      std::cerr << "Variables values: [";
      for (size_t i = 0; i < xs.size(); ++i) {
        if (i != 0)
          std::cerr << ", ";
        std::cerr << xs[i];
      }
      std::cerr << "]" << std::endl;
    }

    return true;
  }

private:
  static double Zap(double x) { return fabs(x) < kEps ? 0.0 : x; }

  // Returns index of the lexicographically smallest row among m_as[i] / m_as[i][j],
  // where i > 0 and m_as[i][j] > 0. If no such row returns 0.
  size_t GetRowToPivot(size_t j) {
    assert(m_trial.size() == m_as.Width());

    size_t p = 0;
    size_t s = 0;
    for (size_t i = 1; i < m_as.Height(); ++i) {
      if (m_as(i, j) <= 0)
        continue;

      if (p == 0) {
        p = i;
        continue;
      }

      for (size_t q = 0; q < m_as.Width(); ++q) {
        if (q == s)
          m_trial[s++] = m_as(p, q) / m_as(p, j);

        const double z = m_as(i, q) / m_as(i, j);
        if (m_trial[q] != z) {
          if (m_trial[q] > z) {
            p = i;
            m_trial[q] = z;
            s = q + 1;
          }
          break;
        }
      }
    }

    return p;
  }

  void Pivot(size_t i, size_t j) {
    assert(i > 0);
    assert(i < m_as.Height());
    assert(j < m_as.Width());

    const auto z = m_as(i, j);
    assert(z > 0);

    for (size_t q = 0; q < m_as.Width(); ++q)
      m_as(i, q) /= z;
    m_as(i, j) = 1;

    for (size_t p = 0; p < m_as.Height(); ++p) {
      if (p == i)
        continue;
      const auto z = m_as(p, j);
      for (size_t q = 0; q < m_as.Width(); ++q)
        m_as(p, q) = Zap(m_as(p, q) - z * m_as(i, q));
      m_as(p, j) = 0;
    }

    m_brow[m_bcol[i]] = 0;
    m_bcol[i] = j;
    m_brow[j] = i;
  }

  void Display() const {
    const auto flags = std::cerr.flags();
    std::cerr << std::setprecision(3);
    for (size_t i = 0; i < m_as.Height(); ++i) {
      for (size_t j = 0; j < m_as.Width(); ++j)
        std::cerr << std::setw(6) << m_as(i, j);
      std::cerr << std::endl;
    }
    std::cerr.flags(flags);
  }

  Matrix<double> m_as;
  std::vector<double> m_trial;

  std::vector<size_t> m_brow;  // basis row for each column
  std::vector<size_t> m_bcol;  // basis column for each row

  bool m_verbose;
};

}  // namespace algo
