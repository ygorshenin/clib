#pragma once

#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <utility>

namespace algo {
template <typename Key, typename Value, typename Priority>
class TreapMap {
 public:
  TreapMap() : root_(nullptr), size_(0) {}

  ~TreapMap() { Destroy(root_); }

  template <typename K, typename V, typename P>
  void Insert(K&& k, V&& v, P&& p) {
    Node* node =
        new Node(std::forward<K>(k), std::forward<V>(v), std::forward<P>(p));
    root_ = Insert(root_, node);
    ++size_;
  }

  bool Erase(const Key& key) {
    Node** pos = Lookup(key);
    Node* node = *pos;
    if (!node)
      return false;
    *pos = Merge(node->left, node->right);
    --size_;
    delete node;
    return true;
  }

  bool Contains(const Key& key) const {
    Node* const* pos = Lookup(key);
    return *pos != nullptr;
  }

  Value& Get(const Key& key) {
    Node** pos = Lookup(key);
    assert(*pos != nullptr);
    return (*pos)->value;
  }

  const Value& Get(const Key& key) const {
    Node** pos = Lookup(key);
    assert(*pos != nullptr);
    return (*pos)->value;
  }

  size_t Size() const { return size_; }

  void Print(std::ostream& os) const { Print(os, root_, 0 /* offset */); }

 private:
  struct Node {
    template <typename K, typename V, typename P>
    Node(K&& k, V&& v, P&& p)
        : key(std::forward<K>(k)),
          value(std::forward<V>(v)),
          priority(std::forward<P>(p)),
          left(nullptr),
          right(nullptr) {}

    Key key;
    Value value;
    Priority priority;

    Node* left;
    Node* right;
  };

  Node* const* Lookup(const Key& key) const {
    Node* const* cur = &root_;
    while (*cur && (*cur)->key != key) {
      if ((*cur)->key < key)
        cur = &(*cur)->right;
      else
        cur = &(*cur)->left;
    }
    return cur;
  }

  Node** Lookup(const Key& key) {
    Node** cur = &root_;
    while (*cur && (*cur)->key != key) {
      if ((*cur)->key < key)
        cur = &(*cur)->right;
      else
        cur = &(*cur)->left;
    }
    return cur;
  }

  static Node* Insert(Node* root, Node* node) {
    if (!root)
      return node;
    if (root->priority >= node->priority) {
      Split(root, node->key, &node->left, &node->right);
      return node;
    }
    if (root->key < node->key)
      root->right = Insert(root->right, node);
    else
      root->left = Insert(root->left, node);
    return root;
  }

  static void Split(Node* root, const Key& key, Node** left, Node** right) {
    if (!root) {
      *left = *right = nullptr;
      return;
    }
    if (root->key < key) {
      *left = root;
      Split((*left)->right, key, &(*left)->right, right);
    } else {
      *right = root;
      Split((*right)->left, key, left, &(*right)->left);
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

  static void Destroy(Node* node) {
    if (!node)
      return;
    Destroy(node->left);
    Destroy(node->right);
    delete node;
  }

  static void Print(std::ostream& os, const Node* node, int offset) {
    if (!node)
      return;
    Print(os, node->right, offset + 1);

    for (int i = 0; i < offset; ++i)
      os << '\t';
    os << "[" << node->key << ", " << node->value << ", " << node->priority
       << "]" << std::endl;

    Print(os, node->left, offset + 1);
  }

  Node* root_;
  size_t size_;
};
}  // namespace algo
