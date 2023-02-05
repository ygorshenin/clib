#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

namespace algo::solvers {
class DLX {
public:
  static constexpr uint32_t INF = std::numeric_limits<uint32_t>::max();

  struct Task {
    uint32_t NumItems() const;
    uint32_t NumOptions() const;

    std::vector<std::vector<uint32_t>> m_options;
  };

  struct Node {
    Node() = default;
    Node(uint32_t ulink, uint32_t dlink, uint32_t top) : m_ulink{ulink}, m_dlink{dlink}, m_top{top} {}

    uint32_t m_ulink{INF};
    uint32_t m_dlink{INF};
    uint32_t m_top{INF};
  };

  std::vector<Node> PrepareTable(const Task& task) const;
  void DebugPrint(std::ostream& os, const Task& task, const std::vector<Node>& table) const;

  template <typename Fn>
  void Solve(const Task& task, Fn&& fn) const {}
};
}  // namespace algo::solvers
