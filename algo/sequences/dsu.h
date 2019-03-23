#pragma once

#include <algorithm>
#include <cstddef>
#include <vector>

namespace algo {
struct DSU {
  explicit DSU(size_t n) : m_parent(n), m_rank(n), m_numComponents(n) {
    for (size_t i = 0; i < n; ++i)
      m_parent[i] = i;
  }

  void Clear() {
    for (size_t i = 0; i < m_parent.size(); ++i)
      m_parent[i] = i;
    std::fill(m_rank.begin(), m_rank.end(), 0);
    m_numComponents = m_parent.size();
  }

  size_t GetParent(size_t u) {
    if (m_parent[u] != u)
      m_parent[u] = GetParent(m_parent[u]);
    return m_parent[u];
  }

  bool Union(size_t u, size_t v) {
    u = GetParent(u);
    v = GetParent(v);
    if (u == v)
      return false;

    if (m_rank[u] < m_rank[v]) {
      m_parent[u] = v;
    } else {
      m_parent[v] = u;
      if (m_rank[u] == m_rank[v])
        ++m_rank[u];
    }

    --m_numComponents;

    return true;
  }

  size_t NumComponents() const { return m_numComponents; }

  std::vector<size_t> m_parent;
  std::vector<size_t> m_rank;
  size_t m_numComponents;
};
}  // namespace algo
