#include <gtest/gtest.h>

#include <algorithm>
#include <cassert>
#include <vector>
#include <limits>

#include "algo/sequences/rmq.h"

namespace algo {

template <typename Value>
Value NaiveRMQ(size_t from, size_t to, const Value* values) {
  assert(to >= from);
  Value min = std::numeric_limits<Value>::max();
  for (; from != to; ++from)
    min = std::min(min, values[from]);
  return min;
}

TEST(RMQ, Empty) {
  std::vector<int> v;
  RMQIndex<int> index;
  BuildRMQIndex(v.size(), v.data(), index);
  ASSERT_EQ(NaiveRMQ(0, 0, v.data()), RMQ(0, 0, index));
}

TEST(RMQ, Simple) {
  std::vector<int> v = {3, 5, 1, 0, 7, 6, 9};
  RMQIndex<int> index;
  BuildRMQIndex(v.size(), v.data(), index);

  for (size_t from = 0; from < v.size(); ++from) {
    for (size_t to = from; to < v.size(); ++to)
      ASSERT_EQ(NaiveRMQ(from, to, v.data()), RMQ(from, to, index));
  }
}

}  // namespace algo
