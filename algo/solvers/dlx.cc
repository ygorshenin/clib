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
void EmitLine(ostream& os, const char* const tag, bool first, It begin, It end, size_t n, Fn&& getter) {
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
void EmitLine(ostream& os, It begin, It end, size_t n, size_t offset) {
  using Node = algo::solvers::DLX::Node;

  const bool first = (offset == 0);

  auto emitLine = [&](const char* tag, auto&& getter) { EmitLine(os, tag, first, begin, end, n, getter); };

  emitLine("\t", [&](const Node& /* node */) { return offset++; });
  emitLine("ULINK:", [&](const Node& node) { return node.m_ulink; });
  emitLine("DLINK:", [&](const Node& node) { return node.m_dlink; });
  emitLine("TOP:", [&](const Node& node) { return node.m_top; });
  os << endl;
}
}  // namespace

namespace algo::solvers {
// DLX::Task -------------------------------------------------------------------
uint32_t DLX::Task::NumItems() const {
  uint32_t numItems = 0;
  for (const auto& option : m_options) {
    for (const auto& item : option)
      numItems = max(numItems, item + 1);
  }
  return numItems;
}

uint32_t DLX::Task::NumOptions() const { return m_options.size(); }

// DLX -------------------------------------------------------------------------
vector<DLX::Node> DLX::PrepareTable(const Task& task) const {
  const auto n = task.NumItems();

  vector<Node> table;
  for (uint32_t i = 0; i < n; ++i)
    table.emplace_back(/* ulink= */ i, /* dlink= */ i, /* top= */ i);

  table.emplace_back();  // a spacer node

  for (auto option : task.m_options) {
    SortUnique(option);
    if (option.empty())
      continue;

    const auto prevSpacer = table.size() - 1;
    for (const auto& item : option) {
      const Node node{/* ulink= */ table[item].m_dlink, /* dlink= */ item, /* top= */ item};
      const uint32_t curr = table.size();
      table.push_back(node);
      table[node.m_dlink].m_ulink = curr;
      table[node.m_ulink].m_dlink = curr;
    }
    table.emplace_back();  // a spacer node
    const auto nextSpacer = table.size() - 1;

    table[prevSpacer].m_dlink = nextSpacer - 1;
    table[nextSpacer].m_ulink = prevSpacer + 1;
  }

  return table;
}

void DLX::DebugPrint(std::ostream& os, const Task& task, const std::vector<Node>& table) const {
  const auto n = task.NumItems();
  const auto m = task.NumOptions();
  os << "Num items: " << n << endl;
  os << "Num options: " << m << endl;

  EmitLine(os, &table[0], &table[0] + n, n, 0);

  uint32_t from = n;
  assert(table[from].m_top == INF);

  while (from + 1 < table.size()) {
    uint32_t to = from + 1;
    while (table[to].m_top != INF)
      ++to;
    EmitLine(os, &table[from], &table[to] + 1, n, from);
    from = to;
  }
}
}  // namespace algo::solvers
