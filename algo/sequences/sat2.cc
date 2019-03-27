#include "sequences/sat2.h"

#include <algorithm>
#include <cassert>
#include <limits>

using namespace std;

namespace algo {
namespace {
enum class Value { Undefined, False, True };

size_t GetIndex(const DNF::X& x) { return 2 * x.m_index + (x.m_positive ? 0 : 1); }

struct SCC {
public:
  inline static constexpr size_t kInvalidIndex = numeric_limits<size_t>::max();

  SCC(const vector<vector<size_t>>& adj) : m_adj(adj) {
    const auto n = m_adj.size();
    m_vis.assign(n, kInvalidIndex);
    m_low.assign(n, kInvalidIndex);
    m_onStack.assign(n, false);

    for (size_t u = 0; u < n; ++u) {
      assert(m_stack.empty());
      if (m_vis[u] == kInvalidIndex)
        DFS(u);
    }
    assert(m_stack.empty());
  }

  size_t Size() const { return m_components.size(); }

  const vector<size_t>& operator[](size_t i) const {
    assert(i < Size());
    return m_components[i];
  }

private:
  void DFS(size_t u) {
    assert(m_vis[u] == kInvalidIndex);
    assert(m_low[u] == kInvalidIndex);

    m_vis[u] = m_low[u] = m_index++;
    Push(u);

    for (const auto v : m_adj[u]) {
      if (m_vis[v] == kInvalidIndex) {
        DFS(v);
        m_low[u] = min(m_low[u], m_low[v]);
      } else if (m_onStack[v]) {
        m_low[u] = min(m_low[u], m_vis[v]);
      }
    }

    if (m_low[u] == m_vis[u]) {
      assert(!m_stack.empty());

      m_components.emplace_back();
      auto& component = m_components.back();

      size_t v;
      do {
        v = Pop();
        component.push_back(v);
      } while (u != v);
    }
  }

  void Push(size_t u) {
    assert(!m_onStack[u]);
    m_onStack[u] = true;
    m_stack.push_back(u);
  }

  size_t Pop() {
    assert(!m_stack.empty());
    const auto u = m_stack.back();
    m_stack.pop_back();
    m_onStack[u] = false;
    return u;
  }

  const vector<vector<size_t>>& m_adj;
  vector<size_t> m_vis;
  vector<size_t> m_low;
  vector<bool> m_onStack;
  vector<size_t> m_stack;

  size_t m_index = 0;

  vector<vector<size_t>> m_components;
};
}  // namespace

ostream& operator<<(ostream& os, const DNF::X& x) {
  if (x.m_positive)
    os << "x";
  else
    os << "~x";
  os << x.m_index;
  return os;
}

ostream& operator<<(ostream& os, const DNF::Disjunction& d) {
  os << d.m_lhs << " or " << d.m_rhs;
  return os;
}

SAT2::SAT2(const DNF& dnf) {
  size_t maxIndex = 0;
  for (const auto& d : dnf.m_ds) {
    maxIndex = max(maxIndex, d.m_lhs.m_index);
    maxIndex = max(maxIndex, d.m_rhs.m_index);
  }

  const size_t n = dnf.Empty() ? 0 : maxIndex + 1;

  m_adj.resize(2 * n);
  for (const auto& d : dnf.m_ds) {
    AddImpl(!d.m_lhs, d.m_rhs);
    AddImpl(!d.m_rhs, d.m_lhs);
  }

  m_assignment.assign(n, false);

  SCC scc{m_adj};

  vector<Value> values(2 * n, Value::Undefined);
  for (size_t i = 0; i < scc.Size(); ++i) {
    const auto& component = scc[i];
    assert(!component.empty());

    const auto u = component.front();
    assert((values[u] == Value::Undefined) == (values[u ^ 1] == Value::Undefined));

    if (values[u] != Value::Undefined)
      continue;

    for (const auto u : component) {
      if (values[u] != Value::Undefined) {
        m_unsat = true;
        return;
      }
      values[u] = Value::True;
      values[u ^ 1] = Value::False;
    }
  }

  for (size_t i = 0; i < values.size(); i += 2) {
    assert(values[i] != Value::Undefined);
    assert(values[i + 1] != Value::Undefined);
    assert(values[i] != values[i + 1]);
    if (values[i] == Value::True)
      m_assignment[i / 2] = true;
  }
}

void SAT2::AddImpl(const DNF::X& a, const DNF::X& b) { m_adj[GetIndex(a)].push_back(GetIndex(b)); }
}  // namespace algo
