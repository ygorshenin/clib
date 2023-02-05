#pragma once

#include <array>
#include <cassert>
#include <cctype>
#include <iterator>
#include <vector>

namespace algo::solvers {
template <size_t N, size_t M>
struct WordRectangle {
  static constexpr size_t kAlphabetSize = 'z' - 'a' + 1;

  struct Trie {
    Trie() : m_nodes(1) {}

    struct Node {
      std::array<size_t, kAlphabetSize> m_moves{};
      unsigned m_mask{};
    };

    template <typename It>
    size_t Add(It begin, It end) {
      size_t curr = 0;

      for (auto it = begin; it != end; ++it) {
        auto c = *it;
        assert(isalpha(c));
        assert(islower(c));
        c -= 'a';
        if (m_nodes[curr].m_moves[c] == 0) {
          const auto next = m_nodes.size();
          m_nodes.emplace_back();
          m_nodes[curr].m_moves[c] = next;
          m_nodes[curr].m_mask |= 1U << c;
        }
        curr = m_nodes[curr].m_moves[c];
      }

      return curr;
    }

    std::vector<Node> m_nodes;
  };

  template <typename NIt, typename MIt, typename Fn>
  void Build(NIt nbegin, NIt nend, MIt mbegin, MIt mend, Fn&& fn) {
    std::vector<std::string> nwords;
    nwords.reserve(std::distance(nbegin, nend));
    for (auto it = nbegin; it != nend; ++it) {
      const auto& word = *it;
      const auto begin = std::begin(word);
      const auto end = std::end(word);
      assert(std::distance(begin, end) == N);
      nwords.emplace_back(begin, end);
      for (auto& c : nwords.back()) {
        assert(isalpha(c));
        assert(islower(c));
        c -= 'a';
      }
    }

    Trie trie;
    for (auto it = mbegin; it != mend; ++it) {
      const auto& word = *it;
      const auto begin = std::begin(word);
      const auto end = std::end(word);
      assert(std::distance(begin, end) == M);
      trie.Add(begin, end);
    }

    std::array<std::array<size_t, M + 1>, N> rows{};
    std::array<size_t, M> col{};
    std::array<size_t, M> curr{};
    size_t level = 0;

  enter:
    if (level == M) {
      fn(col);
      goto downgrade;
    }

    curr[level] = 0;

  loop : {
    if (curr[level] == nwords.size())
      goto downgrade;

    const auto& word = nwords[curr[level]];
    col[level] = curr[level]++;

    bool ok = true;
    for (size_t i = 0; i < N; ++i) {
      const auto c = word[i];

      assert(c < kAlphabetSize);

      const auto curr = rows[i][level];
      const auto next = trie.m_nodes[curr].m_moves[c];
      if (next == 0) {
        ok = false;
        break;
      }
      rows[i][level + 1] = next;
    }

    if (ok) {
      ++level;
      goto enter;
    }

    goto loop;
  }

  downgrade:
    if (level == 0)
      goto finish;
    --level;
    goto loop;

  finish:
    return;
  }
};
}  // namespace algo::solvers
