#include <gtest/gtest.h>

#include <memory>

#include "algo/sequences/heap.h"

namespace {
// A simple movable-only wrapper around int.
class Foo {
public:
  Foo(int value) : m_value(new int(value)) {}
  Foo(Foo &&foo) : m_value(std::move(foo.m_value)) {}

  Foo(const Foo &foo) = delete;

  Foo &operator=(Foo &&foo) {
    m_value = std::move(foo.m_value);
    return *this;
  }

  bool operator<(const Foo &rhs) const { return Value() < rhs.Value(); }

  int Value() const { return *m_value; }

private:
  std::unique_ptr<int> m_value;
};
} // namespace

TEST(Heap, Smoke) {
  algo::Heap<Foo> heap;
  ASSERT_EQ(static_cast<size_t>(0), heap.size());
  ASSERT_TRUE(heap.empty());

  heap.push(Foo(2));
  ASSERT_EQ(static_cast<size_t>(1), heap.size());
  ASSERT_FALSE(heap.empty());
  ASSERT_EQ(2, heap.min().Value());

  heap.push(Foo(1));
  ASSERT_EQ(static_cast<size_t>(2), heap.size());
  ASSERT_FALSE(heap.empty());
  ASSERT_EQ(1, heap.min().Value());

  heap.push(Foo(0));
  ASSERT_EQ(static_cast<size_t>(3), heap.size());
  ASSERT_FALSE(heap.empty());
  ASSERT_EQ(0, heap.min().Value());

  for (int i = 0; i < 3; ++i) {
    Foo &foo = heap.min();
    ASSERT_EQ(i, foo.Value());
    heap.pop();
  }

  ASSERT_EQ(static_cast<size_t>(0), heap.size());
  ASSERT_TRUE(heap.empty());
}
