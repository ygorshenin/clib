#include "algo/sequences/sat2.h"

#include <algorithm>
#include <cassert>
#include <limits>

#include <iostream>

using namespace std;

namespace algo {
namespace {
size_t GetIndex(const DNF::X& x) {
  return 2 * x.index_ + (x.positive_ ? 0 : 1);
}

struct SCC {
 public:
  static const size_t kInvalidIndex = numeric_limits<size_t>::max();

  SCC(const vector<vector<size_t>>& adj) : adj_(adj) {
    const auto n = adj_.size();
    vis_.assign(n, kInvalidIndex);
    low_.assign(n, kInvalidIndex);
    onStack_.assign(n, false);

    for (size_t u = 0; u < n; ++u) {
      assert(stack_.empty());
      if (vis_[u] == kInvalidIndex)
        DFS(u);
    }
    assert(stack_.empty());
  }

  size_t size() const { return components_.size(); }

  const vector<size_t>& operator[](size_t i) const {
    assert(i < size());
    return components_[i];
  }

 private:
  void DFS(size_t u) {
    assert(vis_[u] == kInvalidIndex);
    assert(low_[u] == kInvalidIndex);

    vis_[u] = low_[u] = index_++;
    Push(u);

    for (const auto v : adj_[u]) {
      if (vis_[v] == kInvalidIndex) {
        DFS(v);
        low_[u] = min(low_[u], low_[v]);
      } else if (onStack_[v]) {
        low_[u] = min(low_[u], vis_[v]);
      }
    }

    if (low_[u] == vis_[u]) {
      assert(!stack_.empty());

      components_.emplace_back();
      auto& component = components_.back();

      size_t v;
      do {
        v = Pop();
        component.push_back(v);
      } while (u != v);
    }
  }

  void Push(size_t u) {
    assert(!onStack_[u]);
    onStack_[u] = true;
    stack_.push_back(u);
  }

  size_t Pop() {
    assert(!stack_.empty());
    const auto u = stack_.back();
    stack_.pop_back();
    onStack_[u] = false;
    return u;
  }

  const vector<vector<size_t>>& adj_;
  vector<size_t> vis_;
  vector<size_t> low_;
  vector<bool> onStack_;
  vector<size_t> stack_;

  size_t index_ = 0;

  vector<vector<size_t>> components_;
};

// static
const size_t SCC::kInvalidIndex;

enum Value { VALUE_UNDEFINED, VALUE_FALSE, VALUE_TRUE };

void FillFalse(const vector<size_t>& component, vector<Value>& values) {
  assert(!component.empty());
  if (values[component.front()] != VALUE_UNDEFINED)
    return;

  for (const auto u : component)
    values[u] = VALUE_FALSE;
}
}  // namespace

ostream& operator<<(ostream& os, const DNF::X& x) {
  if (x.positive_)
    os << "x";
  else
    os << "~x";
  os << x.index_;
  return os;
}

ostream& operator<<(ostream& os, const DNF::Disjunction& d) {
  os << d.lhs_ << " or " << d.rhs_;
  return os;
}

SAT2::SAT2(const DNF& dnf) {
  size_t max_index = 0;
  for (const auto& d : dnf.ds_) {
    max_index = max(max_index, d.lhs_.index_);
    max_index = max(max_index, d.rhs_.index_);
  }

  size_t n = dnf.Empty() ? 0 : max_index + 1;

  adj_.resize(2 * n);
  for (const auto& d : dnf.ds_) {
    AddImpl(!d.lhs_, d.rhs_);
    AddImpl(!d.rhs_, d.lhs_);
  }

  assignment_.assign(n, false);

  vector<size_t> component_ids(2 * n, SCC::kInvalidIndex);

  SCC scc(adj_);
  for (size_t i = 0; i < scc.size(); ++i) {
    for (const auto u : scc[i])
      component_ids[u] = i;
  }

  for (size_t i = 0; i < component_ids.size(); i += 2) {
    if (component_ids[i] == component_ids[i + 1]) {
      unsat_ = true;
      return;
    }
  }

  vector<Value> values(2 * n, VALUE_UNDEFINED);
  for (size_t i = 0; i < scc.size(); ++i) {
    const auto& component = scc[i];
    assert(!component.empty());

    const auto u = component.front();
    if (values[u] != VALUE_UNDEFINED && values[u ^ 1] != VALUE_UNDEFINED)
      continue;

    for (const auto u : component) {
      values[u] = VALUE_TRUE;
      FillFalse(scc[component_ids[u ^ 1]], values);
    }
  }

  for (size_t i = 0; i < values.size(); i += 2) {
    assert(values[i] != VALUE_UNDEFINED);
    assert(values[i + 1] != VALUE_UNDEFINED);
    assert(values[i] != values[i + 1]);
    if (values[i] == VALUE_TRUE)
      assignment_[i / 2] = true;
  }
}

void SAT2::AddImpl(const DNF::X& a, const DNF::X& b) {
  adj_[GetIndex(a)].push_back(GetIndex(b));
}
}  // namespace algo
