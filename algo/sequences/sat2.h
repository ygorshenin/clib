#pragma once

#include <cstddef>
#include <ostream>
#include <vector>

namespace algo {
struct DNF {
  struct X {
    explicit X(size_t index) : m_index(index) {}
    X(size_t index, bool positive) : m_index(index), m_positive(positive) {}

    const size_t m_index;
    const bool m_positive = true;
  };

  struct Disjunction {
    Disjunction(const X& x) : m_lhs(x), m_rhs(x) {}
    Disjunction(const X& lhs, const X& rhs) : m_lhs(lhs), m_rhs(rhs) {}

    const X m_lhs;
    const X m_rhs;
  };

  DNF() = default;
  DNF(const X& x) { Add(x); }
  DNF(const Disjunction& d) { Add(d); }
  DNF(const std::vector<Disjunction>& ds) {
    for (const auto& d : ds)
      Add(d);
  }

  void Add(const X& x) { m_ds.emplace_back(x, x); }
  void Add(const Disjunction& d) { m_ds.push_back(d); };

  DNF& operator&=(const X& x) {
    Add(x);
    return *this;
  }
  DNF& operator&=(const Disjunction& d) {
    Add(d);
    return *this;
  }

  bool Empty() const { return m_ds.empty(); }

  std::vector<Disjunction> m_ds;
};

inline DNF::X operator!(const DNF::X& x) { return {x.m_index, !x.m_positive}; }
inline DNF::Disjunction operator|(const DNF::X& lhs, const DNF::X& rhs) { return {lhs, rhs}; }
inline DNF::Disjunction operator>=(const DNF::X& lhs, const DNF::X& rhs) { return {!lhs, rhs}; }
inline DNF operator&(const DNF& lhs, const DNF::Disjunction& rhs) {
  DNF result = lhs;
  result.Add(rhs);
  return result;
}

std::ostream& operator<<(std::ostream& os, const DNF::X& x);
std::ostream& operator<<(std::ostream& os, const DNF::Disjunction& d);

class SAT2 {
public:
  SAT2() = default;
  SAT2(const DNF& dnf);

  bool Unsat() const { return m_unsat; }

  const std::vector<bool>& Assignment() const { return m_assignment; }

private:
  void AddImpl(const DNF::X& a, const DNF::X& b);

  std::vector<std::vector<size_t>> m_adj;
  std::vector<bool> m_assignment;
  bool m_unsat = false;
};
}  // namespace algo
