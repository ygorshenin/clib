#pragma once

#include "bits/bits.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <optional>

namespace algo {
struct DictionaryLeaf {
  void Set(uint64_t i) {
    assert(i < 64);
    m_bits = m_bits | (static_cast<uint64_t>(1) << i);
  }

  void Clear(uint64_t i) {
    assert(i < 64);
    m_bits = m_bits & ~(static_cast<uint64_t>(1) << i);
  }

  bool Get(uint64_t i) const {
    assert(i < 64);
    return (m_bits >> i) & 1;
  }

  bool Empty() const { return m_bits == 0; }

  std::optional<uint64_t> Min() const {
    if (Empty())
      return {};
    return LoPosUnsafe(m_bits);
  }

  std::optional<uint64_t> Max() const {
    if (Empty())
      return {};
    return HiPosUnsafe(m_bits);
  }

  std::optional<uint64_t> Pred(uint64_t i) const {
    const auto bits = m_bits & ((static_cast<uint64_t>(1) << i) - 1);
    if (bits == 0)
      return {};
    return HiPosUnsafe(bits);
  }

  std::optional<uint64_t> Succ(uint64_t i) const {
    auto bits = m_bits >> i;
    bits >>= 1;
    if (bits == 0)
      return {};
    return LoPosUnsafe(bits) + i + 1;
  }

  uint64_t m_bits{};
};

template <uint64_t LogSize>
struct Dictionary {
  static_assert(LogSize > 6);

  static constexpr uint64_t POW_NUM_CHILDREN = LogSize % 6 == 0 ? 6 : LogSize % 6;
  static constexpr uint64_t POW_BLOCK_SIZE = LogSize - POW_NUM_CHILDREN;

  static constexpr uint64_t NUM_CHILDREN = static_cast<uint64_t>(1) << POW_NUM_CHILDREN;
  static constexpr uint64_t BLOCK_SIZE = static_cast<uint64_t>(1) << POW_BLOCK_SIZE;

  // Sets |i|-th bit.
  // Complexity: O(LogSize).
  void Set(uint64_t i) {
    const auto block = i / BLOCK_SIZE;
    const auto offset = i % BLOCK_SIZE;
    m_children[block].Set(offset);
    m_aux.Set(block);
  }

  // Clears |i|-th bit.
  // Complexity: O(LogSize).
  void Clear(uint64_t i) {
    const auto block = i / BLOCK_SIZE;
    const auto offset = i % BLOCK_SIZE;
    m_children[block].Clear(offset);
    if (m_children[block].Empty())
      m_aux.Clear(block);
  }

  // Gets state of the |i|-th bit.
  // Complexity: O(LogSize).
  bool Get(uint64_t i) const {
    const auto block = i / BLOCK_SIZE;
    const auto offset = i % BLOCK_SIZE;
    return m_children[block].Get(offset);
  }

  // Returns true iff the dictionary is empty.
  // Complexity: O(1).
  bool Empty() const { return m_aux.Empty(); }

  // Returns min value from the dictionary.
  // Complexity: O(LogSize).
  std::optional<uint64_t> Min() const {
    if (const auto block = m_aux.Min())
      return *m_children[*block].Min() + (*block) * BLOCK_SIZE;
    return {};
  }

  // Returns max value from the dictionary.
  // Complexity: O(LogSize).
  std::optional<uint64_t> Max() const {
    if (const auto block = m_aux.Max())
      return *m_children[*block].Max() + (*block) * BLOCK_SIZE;
    return {};
  }

  // Returns rightmost set bit with position less than |i|.
  // Complexity: O(LogSize * LogSize).
  std::optional<uint64_t> Pred(uint64_t i) const {
    const auto block = i / BLOCK_SIZE;
    const auto offset = i % BLOCK_SIZE;

    if (const auto pred = m_children[block].Pred(offset))
      return *pred + block * BLOCK_SIZE;

    if (const auto pred = m_aux.Pred(block))
      return *m_children[*pred].Max() + (*pred) * BLOCK_SIZE;

    return {};
  }

  // Returns leftmost set bit with position greater than |i|.
  // Complexity: O(LogSize * LogSize).
  std::optional<uint64_t> Succ(uint64_t i) const {
    const auto block = i / BLOCK_SIZE;
    const auto offset = i % BLOCK_SIZE;

    if (const auto succ = m_children[block].Succ(offset))
      return *succ + block * BLOCK_SIZE;

    if (const auto succ = m_aux.Succ(block))
      return *m_children[*succ].Min() + (*succ) * BLOCK_SIZE;

    return {};
  }

  std::array<Dictionary<POW_BLOCK_SIZE>, NUM_CHILDREN> m_children;
  DictionaryLeaf m_aux;
};

template <>
struct Dictionary<6> : public DictionaryLeaf {};
}  // namespace algo
