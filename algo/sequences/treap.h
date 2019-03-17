#pragma once

#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

namespace algo {
template <typename Key, typename Value, typename Priority>
struct TreapNode {
  template <typename K, typename V, typename P>
  TreapNode(K&& k, V&& v, P&& p)
      : key(std::forward<K>(k)),
        value(std::forward<V>(v)),
        priority(std::forward<P>(p)),
        left(nullptr),
        right(nullptr) {}

  ~TreapNode() {
    delete left;
    delete right;
  }

  inline void Detach() { left = right = nullptr; }

  void Print(std::ostream& os, int offset) {
    if (right)
      right->Print(os, offset + 1);

    for (int i = 0; i < offset; ++i)
      os << '\t';
    os << "[" << key << ", " << value << ", " << priority << "]" << std::endl;

    if (left)
      left->Print(os, offset + 1);
  }

  Key key;
  Value value;
  Priority priority;

  TreapNode<Key, Value, Priority>* left;
  TreapNode<Key, Value, Priority>* right;
};

template <typename Key, typename Priority>
struct TreapNode<Key, void, Priority> {
  template <typename K, typename P>
  TreapNode(K&& k, P&& p) : key(std::forward<K>(k)), priority(std::forward<P>(p)), left(nullptr), right(nullptr) {}

  ~TreapNode() {
    delete left;
    delete right;
  }

  inline void Detach() { left = right = nullptr; }

  void Print(std::ostream& os, int offset) {
    if (right)
      right->Print(os, offset + 1);

    for (int i = 0; i < offset; ++i)
      os << '\t';
    os << "[" << key << ", " << priority << "]" << std::endl;

    if (left)
      left->Print(os, offset + 1);
  }

  Key key;
  Priority priority;

  TreapNode<Key, void, Priority>* left;
  TreapNode<Key, void, Priority>* right;
};

template <typename Key, typename Value, typename Priority>
struct Treap {
  using Node = TreapNode<Key, Value, Priority>;

  Treap() : root_(nullptr), size_(0) {}

  ~Treap() { delete root_; }

  template <typename K, typename P>
  void Insert(K&& k, P&& p) {
    Node* node = new Node(std::forward<K>(k), std::forward<P>(p));
    Erase(node->key);
    root_ = Insert(root_, node);
    ++size_;
  }

  template <typename K, typename V, typename P>
  void Insert(K&& k, V&& v, P&& p) {
    Node* node = new Node(std::forward<K>(k), std::forward<V>(v), std::forward<P>(p));
    Erase(node->key);
    root_ = Insert(root_, node);
    ++size_;
  }

  bool Erase(const Key& key) {
    Node** pos = Lookup(key);
    std::unique_ptr<Node> node(*pos);
    if (!node)
      return false;
    *pos = Merge(node->left, node->right);
    node->Detach();
    --size_;
    return true;
  }

  bool Contains(const Key& key) const {
    Node* const* pos = Lookup(key);
    return *pos != nullptr;
  }

  Node& Get(const Key& key) {
    Node** pos = Lookup(key);
    assert(*pos != nullptr);
    return **pos;
  }

  const Key& Get(const Key& key) const {
    Node** pos = Lookup(key);
    assert(*pos != nullptr);
    return **pos;
  }

  inline size_t Size() const { return size_; }

  inline bool Empty() const { return Size() == 0; }

  void Print(std::ostream& os) const {
    if (root_)
      root_->Print(os, 0 /* offset */);
  }

#define LOOKUP(cur, root)                 \
  auto cur = &root;                       \
  while (*cur && !((*cur)->key == key)) { \
    if (key < (*cur)->key)                \
      cur = &(*cur)->left;                \
    else                                  \
      cur = &(*cur)->right;               \
  }

  Node* const* Lookup(const Key& key) const {
    LOOKUP(cur, root_);
    return cur;
  }

  Node** Lookup(const Key& key) {
    LOOKUP(cur, root_);
    return cur;
  }
#undef LOOKUP

  static Node* Insert(Node* root, Node* node) {
    if (!root)
      return node;
    if (node->priority < root->priority) {
      Split(root, node->key, &node->left, &node->right);
      return node;
    }
    if (node->key < root->key) {
      root->left = Insert(root->left, node);
    } else {
      root->right = Insert(root->right, node);
    }
    return root;
  }

  static void Split(Node* root, const Key& key, Node** left, Node** right) {
    if (!root) {
      *left = *right = nullptr;
      return;
    }
    if (!(root->key < key)) {
      *right = root;
      Split((*right)->left, key, left, &(*right)->left);
    } else {
      *left = root;
      Split((*left)->right, key, &(*left)->right, right);
    }
  }

  static Node* Merge(Node* left, Node* right) {
    if (!left)
      return right;
    if (!right)
      return left;
    if (left->priority < right->priority) {
      left->right = Merge(left->right, right);
      return left;
    }
    right->left = Merge(left, right->left);
    return right;
  }

  Node* root_;
  size_t size_;
};
}  // namespace algo
