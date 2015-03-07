#pragma once

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

namespace algo {

template <typename Value>
using RMQTable = std::vector<std::vector<Value> >;

template <typename T>
T FloorLog(const T& n) {
  const T one = static_cast<T>(1);
  T p = 0;
  while ((one << (p + 1)) <= n)
    ++p;
  return p;
}

template <typename T>
T CeilLog(const T& n) {
  const T one = static_cast<T>(1);
  T p = 0;
  while ((one << p) < n)
    ++p;
  return p;
}

template <typename Value>
void BuildRMQTable(size_t n, const Value* values, RMQTable<Value>& table) {
  if (!n) {
    table.clear();
    return;
  }
  table.resize(CeilLog(n));
  table[0].assign(values, values + n);
  for (int p = 1; (static_cast<size_t>(1) << p) <= n; ++p) {
    size_t window_size = static_cast<size_t>(1) << p;
    size_t half_window_size = window_size >> 1;

    table[p].resize(n - window_size + 1);
    const std::vector<Value>& prev = table[p - 1];
    std::vector<Value>& cur = table[p];
    for (size_t i = 0; i + window_size <= n; ++i)
      cur[i] = std::min(prev[i], prev[i + half_window_size]);
  }
}

template <typename Value>
Value RMQ(size_t from, size_t to, const RMQTable<Value>& table) {
  assert(to >= from);
  if (from == to)
    return std::numeric_limits<Value>::max();
  size_t size = to - from;
  size_t level = FloorLog(size);
  size_t window = static_cast<size_t>(1) << level;
  assert(table.size() > level);
  if (size == window)
    return table[level][from];
  return std::min(table[level][from], table[level][to - window]);
}

}  // namespace algo
