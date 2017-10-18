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
  ASSERT_EQ(static_cast<size_t>(0), heap.Size());
  ASSERT_TRUE(heap.Empty());

  heap.Push(Foo(2));
  ASSERT_EQ(static_cast<size_t>(1), heap.Size());
  ASSERT_FALSE(heap.Empty());
  ASSERT_EQ(2, heap.Min().Value());

  heap.Push(Foo(1));
  ASSERT_EQ(static_cast<size_t>(2), heap.Size());
  ASSERT_FALSE(heap.Empty());
  ASSERT_EQ(1, heap.Min().Value());

  heap.Push(Foo(0));
  ASSERT_EQ(static_cast<size_t>(3), heap.Size());
  ASSERT_FALSE(heap.Empty());
  ASSERT_EQ(0, heap.Min().Value());

  for (int i = 0; i < 3; ++i) {
    Foo &foo = heap.Min();
    ASSERT_EQ(i, foo.Value());
    heap.Pop();
  }

  ASSERT_EQ(static_cast<size_t>(0), heap.Size());
  ASSERT_TRUE(heap.Empty());
}
