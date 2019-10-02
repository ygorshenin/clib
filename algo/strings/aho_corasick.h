#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace algo {
struct AhoCorasick {
  struct Node {
    Node &GetOrCreateMove(char c) {
      if (m_moves.count(c) == 0)
        m_moves[c] = std::make_unique<Node>();
      return *m_moves[c];
    }

    Node &GetMove(char c) {
      assert(HasMove(c));
      return *m_moves[c];
    }

    bool HasMove(char c) const { return m_moves.count(c) != 0; }

    std::unordered_map<char, std::unique_ptr<Node>> m_moves;
    Node *m_link = nullptr;
    bool m_terminal = false;
  };

  struct Iterator {
    explicit Iterator(Node &root) : m_root(&root), m_curr(&root) {}

    bool IsTerminal() const {
      assert(m_curr);
      return m_curr->m_terminal;
    }

    bool Move(char c) {
      while (m_curr != m_root && !m_curr->HasMove(c))
        m_curr = m_curr->m_link;
      if (m_curr->HasMove(c))
        m_curr = &m_curr->GetMove(c);
      return IsTerminal();
    }

    Node *m_root;
    Node *m_curr;
  };

  AhoCorasick();

  void Add(const std::string &word);
  void Build();

  Iterator Begin() const {
    assert(m_root);
    return Iterator(*m_root);
  }

  std::unique_ptr<Node> m_root;
};
} // namespace algo
