#pragma once

#include <cstddef>
#include <ostream>
#include <vector>

namespace algo {
struct DNF {
  struct X {
    explicit X(size_t index) : index_(index) {}
    X(size_t index, bool positive) : index_(index), positive_(positive) {}

    const size_t index_;
    const bool positive_ = true;
  };

  struct Disjunction {
    Disjunction(const X& x) : lhs_(x), rhs_(x) {}
    Disjunction(const X& lhs, const X& rhs) : lhs_(lhs), rhs_(rhs) {}

    const X lhs_;
    const X rhs_;
  };

  DNF() = default;
  DNF(const std::vector<Disjunction>& ds) {
    for (const auto d : ds)
      Add(d);
  }

  void Add(const X& x) { ds_.emplace_back(x, x); }
  void Add(const Disjunction& d) { ds_.push_back(d); };

  bool Empty() const { return ds_.empty(); }

  std::vector<Disjunction> ds_;
};

inline DNF::X operator!(const DNF::X& x) {
  return {x.index_, !x.positive_};
}

inline DNF::Disjunction operator|(const DNF::X& lhs, const DNF::X& rhs) {
  return {lhs, rhs};
}

inline DNF::Disjunction operator>=(const DNF::X& lhs, const DNF::X& rhs) {
  return {!lhs, rhs};
}

std::ostream& operator<<(std::ostream& os, const DNF::X& x);
std::ostream& operator<<(std::ostream& os, const DNF::Disjunction& d);

class SAT2 {
 public:
  SAT2(const DNF& dnf);

  bool Unsat() const { return unsat_; }

  const std::vector<bool>& Assignment() const { return assignment_; }

 private:
  void AddImpl(const DNF::X& a, const DNF::X& b);

  std::vector<std::vector<size_t>> adj_;
  std::vector<bool> assignment_;
  bool unsat_ = false;
};
}  // namespace algo
