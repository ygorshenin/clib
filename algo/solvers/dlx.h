#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

namespace algo::solvers {
class DLX {
public:
  static constexpr uint32_t INF = std::numeric_limits<uint32_t>::max();

  class Task {
  public:
    explicit Task(uint32_t numStrict, uint32_t numSlack = 0) : m_strict{numStrict}, m_total{numStrict + numSlack} {
      assert(m_total >= m_strict);
    }

    uint32_t NumStrictItems() const { return m_strict; }
    uint32_t NumTotalItems() const { return m_total; }
    uint32_t NumOptions() const { return m_options.size(); }

    void AddOption(std::vector<uint32_t> option) { m_options.emplace_back(std::move(option)); }

    const std::vector<uint32_t>& GetOption(uint32_t i) const { return m_options[i]; }

  private:
    std::vector<std::vector<uint32_t>> m_options;
    uint32_t m_strict{};
    uint32_t m_total{};
  };

  struct Node {
    Node() = default;
    Node(uint32_t ulink, uint32_t dlink, uint32_t top) : m_ulink{ulink}, m_dlink{dlink}, m_top{top} {}

    uint32_t m_ulink{INF};
    uint32_t m_dlink{INF};
    uint32_t m_top{INF};
  };

  struct Head {
    uint32_t m_left{};
    uint32_t m_right{};
    uint32_t m_len{};
  };

  struct Table {
    explicit Table(const Task& task);

    const Node& operator[](uint32_t i) const { return m_table[i]; }
    Node& operator[](uint32_t i) { return m_table[i]; }

    void Remove(uint32_t curr) {
      assert(m_table[curr].m_top != INF);

      const auto ulink = m_table[curr].m_ulink;
      const auto dlink = m_table[curr].m_dlink;
      m_table[ulink].m_dlink = dlink;
      m_table[dlink].m_ulink = ulink;
    }

    void Insert(uint32_t curr) {
      assert(m_table[curr].m_top != INF);

      const auto ulink = m_table[curr].m_ulink;
      const auto dlink = m_table[curr].m_dlink;
      m_table[ulink].m_dlink = curr;
      m_table[dlink].m_ulink = curr;
    }

    template <typename Fn>
    void LoopRight(uint32_t x, Fn&& fn) {
      uint32_t y = x + 1;

      while (y != x) {
        if (m_table[y].m_top == INF) {
          y = m_table[y].m_ulink;
          continue;
        }
        fn(y);
        ++y;
      }
    }

    template <typename Fn>
    void Hide(uint32_t x, Fn&& fn) {
      LoopRight(x, [&](uint32_t y) {
        Remove(y);
        fn(m_table[y].m_top);
      });
    }

    template <typename Fn>
    void LoopLeft(uint32_t x, Fn&& fn) {
      uint32_t y = x - 1;
      while (y != x) {
        if (m_table[y].m_top == INF) {
          y = m_table[y].m_dlink;
          continue;
        }
        fn(y);
        --y;
      }
    }

    template <typename Fn>
    void Unhide(uint32_t x, Fn&& fn) {
      LoopLeft(x, [&](uint32_t y) {
        fn(m_table[y].m_top);
        Insert(y);
      });
    }

    void DebugPrint(std::ostream& os) const;

    friend inline std::ostream& operator<<(std::ostream& os, const Table& table) {
      table.DebugPrint(os);
      return os;
    }

    const Task& m_task;
    std::vector<Node> m_table;
    std::vector<uint32_t> m_options;
  };

  struct HeadList {
    HeadList(uint32_t numStrict, uint32_t numTotal, Table& table);

    const Head& operator[](uint32_t i) const { return m_list[i]; }
    Head& operator[](uint32_t i) { return m_list[i]; }

    bool Empty() const { return m_list[m_head].m_right == m_head; }

    void Remove(uint32_t curr) {
      const auto left = m_list[curr].m_left;
      const auto right = m_list[curr].m_right;
      m_list[left].m_right = right;
      m_list[right].m_left = left;
    }

    void Insert(uint32_t curr) {
      const auto left = m_list[curr].m_left;
      const auto right = m_list[curr].m_right;
      m_list[left].m_right = curr;
      m_list[right].m_left = curr;
    }

    uint32_t GetMin() const {
      auto best = m_list[m_head].m_right;
      for (auto curr = m_list[best].m_right; curr != m_head; curr = m_list[curr].m_right) {
        if (m_list[curr].m_len < m_list[best].m_len)
          best = curr;
      }
      return best;
    }

    void Cover(uint32_t x) {
      for (auto y = m_table[x].m_dlink; y != x; y = m_table[y].m_dlink)
        m_table.Hide(y, [this](uint32_t z) { --m_list[z].m_len; });
      Remove(x);
    }

    void Uncover(uint32_t x) {
      Insert(x);
      for (auto y = m_table[x].m_ulink; y != x; y = m_table[y].m_ulink)
        m_table.Unhide(y, [this](uint32_t z) { ++m_list[z].m_len; });
    }

    void DebugPrint(std::ostream& os) const;

    friend inline std::ostream& operator<<(std::ostream& os, const HeadList& head) {
      head.DebugPrint(os);
      return os;
    }

    Table& m_table;
    std::vector<Head> m_list;
    uint32_t m_head;
  };

  template <bool Decode = true, typename Fn>
  static void Solve(const Task& task, Fn&& fn) {
    const auto m = task.NumOptions();

    std::vector<uint32_t> xs(m + 1);
    std::vector<uint32_t> os(m + 1);

    Table table{task};
    HeadList head{task.NumStrictItems(), task.NumTotalItems(), table};
    size_t level = 0;

  enter : {
    if (head.Empty()) {
      if (Decode)
        fn(os.data(), level);
      else
        fn(os.data(), 0);
      goto downgrade;
    }

    const auto best = head.GetMin();
    head.Cover(best);

    xs[level] = table[best].m_dlink;
  }

  check : {
    if (xs[level] == table[xs[level]].m_top) {
      head.Uncover(xs[level]);
      goto downgrade;
    }

    if (Decode)
      os[level] = table.m_options[xs[level]];
    table.LoopRight(xs[level], [&](uint32_t y) { head.Cover(table[y].m_top); });
    ++level;
    goto enter;
  }

  downgrade : {
    if (level == 0)
      goto finish;
    --level;

    table.LoopLeft(xs[level], [&](uint32_t y) { head.Uncover(table[y].m_top); });
    xs[level] = table[xs[level]].m_dlink;
    goto check;
  }

  finish:
    return;
  }
};
}  // namespace algo::solvers
