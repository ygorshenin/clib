#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>

namespace algo::solvers {
class DLX {
public:
  static constexpr uint32_t INF = std::numeric_limits<uint32_t>::max();

  class Task {
  public:
    explicit Task(uint32_t numItems) : m_slack(numItems), m_numItems{numItems} {}

    uint32_t NumItems() const { return m_numItems; }
    uint32_t NumOptions() const { return m_options.size(); }

    void SetSlack(uint32_t item) {
      assert(item < m_numItems);
      m_slack[item] = true;
    }

    bool IsSlack(uint32_t item) const {
      assert(item < m_numItems);
      return m_slack[item];
    }

    void AddOption(std::vector<uint32_t> option) { m_options.emplace_back(std::move(option)); }

    const std::vector<uint32_t>& GetOption(uint32_t i) const { return m_options[i]; }

  private:
    std::vector<std::vector<uint32_t>> m_options;
    std::vector<bool> m_slack;
    uint32_t m_numItems{};
  };

  struct Range {
    void Update(int32_t value) {
      m_min = std::min(m_min, value);
      m_max = std::max(m_max, value);
    }

    uint32_t Size() const {
      if (m_min > m_max)
        return 0;
      return m_max - m_min + 1;
    }

    int32_t m_min = std::numeric_limits<int32_t>::max();
    int32_t m_max = std::numeric_limits<int32_t>::min();
  };

  template <uint32_t N>
  class DimTask {
  public:
    using Option = std::array<std::vector<int32_t>, N>;

    class OptionBuilder {
    public:
      explicit OptionBuilder(Option& option) : m_option{option} {}

      template <uint32_t Dim, typename... Args>
      OptionBuilder& SetItems(Args&&... args) {
        static_assert(Dim < N);
        m_option[Dim] = {static_cast<int32_t>(args)...};
        return *this;
      }

    private:
      Option& m_option;
    };

    template <typename... Args>
    DimTask(Args&&... args) : m_sizes{static_cast<uint32_t>(args)...}, m_slack{} {}

    template <uint32_t Dim>
    void SetSlack() {
      static_assert(Dim < N);
      m_slack[Dim] = true;
    }

    OptionBuilder AddOption() {
      m_options.emplace_back();
      return OptionBuilder{m_options.back()};
    }

    const Option& GetOption(uint32_t i) const {
      assert(i < m_options.size());
      return m_options[i];
    }

    Task Convert() const {
      std::array<uint32_t, N + 1> offsets{};
      offsets[0] = 0;
      for (uint32_t i = 0; i < N; ++i)
        offsets[i + 1] = offsets[i] + m_sizes[i];

      const uint32_t numItems = offsets[N];

      Task task{numItems};
      for (uint32_t dim = 0; dim < N; ++dim) {
        if (!m_slack[dim])
          continue;
        for (uint32_t i = offsets[dim]; i < offsets[dim + 1]; ++i)
          task.SetSlack(i);
      }

      std::array<Range, N> ranges;
      for (const auto& option : m_options) {
        for (uint32_t dim = 0; dim < N; ++dim) {
          auto& range = ranges[dim];
          for (const auto& item : option[dim])
            range.Update(item);
        }
      }

      for (uint32_t dim = 0; dim < N; ++dim) {
        assert(ranges[dim].Size() <= m_sizes[dim]);
      }

      for (const auto& option : m_options) {
        std::vector<uint32_t> row;
        for (uint32_t dim = 0; dim < N; ++dim) {
          for (const auto& item : option[dim]) {
            const uint32_t pos = static_cast<uint32_t>(item - ranges[dim].m_min) + offsets[dim];
            row.push_back(pos);
          }
        }
        task.AddOption(row);
      }
      return task;
    }

  private:
    std::array<uint32_t, N> m_sizes;
    std::array<bool, N> m_slack;
    std::vector<Option> m_options;
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
    explicit HeadList(Table& table);

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

  template <uint32_t N, typename Fn>
  static void Solve(const DimTask<N>& task, Fn&& fn) {
    return Solve(task.Convert(), std::forward<Fn>(fn));
  }

  template <typename Fn>
  static void Solve(const Task& task, Fn&& fn) {
    const auto m = task.NumOptions();

    std::vector<uint32_t> xs(m + 1);
    std::vector<uint32_t> os(m + 1);

    Table table{task};
    HeadList head{table};
    size_t level = 0;

  enter : {
    if (head.Empty()) {
      fn(os.data(), level);
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
