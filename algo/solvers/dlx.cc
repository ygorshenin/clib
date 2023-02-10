#include <solvers/dlx.h>

#include <algorithm>
#include <cassert>
using namespace std;

namespace {
template <typename T>
void SortUnique(vector<T>& vs) {
  sort(vs.begin(), vs.end());
  vs.erase(unique(vs.begin(), vs.end()), vs.end());
}

template <typename Fn, typename It>
void EmitTableLine(ostream& os, const char* const tag, bool first, It begin, It end, size_t n, Fn&& getter) {
  const auto INF = algo::solvers::DLX::INF;

  auto emit = [&os, &getter](auto it) {
    const auto value = getter(*it);
    os << '\t';
    if (value == INF)
      os << "⊥";
    else
      os << value;
  };

  os << tag;
  if (first)
    os << "\t";

  for (; begin != end && begin->m_top == INF; ++begin)
    emit(begin);

  for (size_t i = 0; i < n; ++i) {
    if (begin != end && i < begin->m_top) {
      os << "\t";
      continue;
    }
    if (begin != end) {
      assert(i == begin->m_top);
      emit(begin);
      ++begin;
      continue;
    }
  }
  for (; begin != end && begin->m_top == INF; ++begin)
    emit(begin);
  os << endl;
}

template <typename It>
void EmitTableLine(ostream& os, It begin, It end, size_t n, size_t offset) {
  using Node = algo::solvers::DLX::Node;

  const bool first = (offset == 0);

  auto emitLine = [&](const char* tag, auto&& getter) { EmitTableLine(os, tag, first, begin, end, n, getter); };

  emitLine("\t", [&](const Node& /* node */) { return offset++; });
  emitLine("ULINK:", [&](const Node& node) { return node.m_ulink; });
  emitLine("DLINK:", [&](const Node& node) { return node.m_dlink; });
  emitLine("TOP:", [&](const Node& node) { return node.m_top; });
  os << endl;
}

template <typename Fn>
void EmitHeadLine(ostream& os, const char* const tag, size_t n, Fn&& getter) {
  os << tag;
  for (size_t i = 0; i < n; ++i)
    os << '\t' << getter(i);
  os << endl;
}
}  // namespace

namespace algo::solvers {
// DLX::Table ------------------------------------------------------------------
DLX::Table::Table(const Task& task) : m_task{task} {
  const auto m = task.NumOptions();
  const auto n = task.NumItems();

  for (uint32_t i = 0; i < n; ++i) {
    m_table.emplace_back(/* ulink= */ i, /* dlink= */ i, /* top= */ i);
    m_options.emplace_back(INF);
  }

  m_table.emplace_back();  // a spacer node
  m_options.emplace_back(INF);

  for (uint32_t i = 0; i < m; ++i) {
    auto option = task.GetOption(i);
    SortUnique(option);
    if (option.empty())
      continue;

    const auto prevSpacer = m_table.size() - 1;
    for (const auto& item : option) {
      assert(item < n);

      const Node node{/* ulink= */ m_table[item].m_ulink, /* dlink= */ item, /* top= */ item};
      const uint32_t curr = m_table.size();
      m_table.push_back(node);
      m_table[node.m_dlink].m_ulink = curr;
      m_table[node.m_ulink].m_dlink = curr;
      m_options.push_back(i);
    }
    m_table.emplace_back();  // a spacer node
    m_options.emplace_back(INF);
    const auto nextSpacer = m_table.size() - 1;

    m_table[prevSpacer].m_dlink = nextSpacer - 1;
    m_table[nextSpacer].m_ulink = prevSpacer + 1;
  }

  assert(m_table.size() == m_options.size());
}

void DLX::Table::DebugPrint(std::ostream& os) const {
  const auto n = m_task.NumItems();
  const auto m = m_task.NumOptions();

  os << "Num items: " << n << endl;

  os << "Slack items:";
  for (uint32_t i = 0; i < n; ++i) {
    if (m_task.IsSlack(i))
      os << ' ' << i;
  }
  os << endl;

  os << "Num options: " << m << endl;

  EmitTableLine(os, &m_table[0], &m_table[0] + n, n, 0);

  uint32_t from = n;
  assert(m_table[from].m_top == INF);

  while (from + 1 < m_table.size()) {
    uint32_t to = from + 1;
    while (m_table[to].m_top != INF)
      ++to;
    EmitTableLine(os, &m_table[from], &m_table[to] + 1, n, from);
    from = to;
  }
}

// DLX::HeadList ---------------------------------------------------------------
DLX::HeadList::HeadList(Table& table)
    : m_table{table}, m_list(table.m_task.NumItems() + 1), m_head{table.m_task.NumItems()} {
  const auto& task = table.m_task;

  m_list[m_head].m_left = m_head;
  m_list[m_head].m_right = m_head;

  for (uint32_t i = 0; i < task.NumItems(); ++i) {
    if (task.IsSlack(i)) {
      m_list[i].m_left = m_list[i].m_right = i;
    } else {
      m_list[i].m_left = m_list[m_head].m_left;
      m_list[i].m_right = m_head;
      Insert(i);
    }
    for (auto curr = table[i].m_dlink; curr != i; curr = table[curr].m_dlink)
      ++m_list[i].m_len;
  }
}

void DLX::HeadList::DebugPrint(std::ostream& os) const {
  auto emitLine = [&](const char* tag, auto&& getter) { EmitHeadLine(os, tag, m_list.size(), getter); };

  emitLine("\t", [](size_t i) { return i; });
  emitLine("LEFT:", [this](size_t i) { return m_list[i].m_left; });
  emitLine("RIGHT:", [this](size_t i) { return m_list[i].m_right; });
  emitLine("LEN:", [this](size_t i) { return m_list[i].m_len; });
}
}  // namespace algo::solvers
