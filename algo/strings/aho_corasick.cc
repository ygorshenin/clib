#include "strings/aho_corasick.h"

#include <cassert>
#include <queue>

namespace algo {

AhoCorasick::AhoCorasick() : m_root(std::make_unique<Node>()) {}

void AhoCorasick::Add(const std::string &word) {
  assert(m_root);

  Node *curr = m_root.get();
  for (const auto &c : word)
    curr = &curr->GetOrCreateMove(c);
  curr->m_terminal = true;
}

void AhoCorasick::Build() {
  assert(m_root);
  m_root->m_link = m_root.get();

  std::queue<Node *> q;
  for (const auto &move : m_root->m_moves) {
    auto *node = move.second.get();
    node->m_link = m_root.get();
    q.emplace(node);
  }

  while (!q.empty()) {
    const auto *curr = q.front();
    q.pop();
    assert(curr);

    for (const auto &move : curr->m_moves) {
      const auto c = move.first;
      auto *next = move.second.get();
      assert(next);

      auto *link = curr->m_link;
      while (link != m_root.get() && !link->HasMove(c))
        link = link->m_link;
      if (link->HasMove(c))
        link = &link->GetMove(c);
      next->m_link = link;
      next->m_terminal = next->m_terminal || link->m_terminal;
      q.push(next);
    }
  }
}

} // namespace algo
