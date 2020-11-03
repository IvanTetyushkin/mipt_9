#pragma once
#include <array>
#include <cassert>
#include <iterator>

#include <array>
#include <cmath>
#include <list>
#include <memory>
#include <variant>
namespace handmade {
#define Debug
// macro Debug
// well, b, b+ and other seems to be too difficult to iterate... (
template <typename T>
class half_avl_tree // do not want to implement delete...)
{
  struct Node;
  using pNode = Node *;
  struct Node {
    T key;
    pNode parent = nullptr;
    int weight = 0;
    pNode left = nullptr;
    pNode right = nullptr;
    Node(T k) : key(k){};
  };
  // only field
  pNode root;
#ifdef Debug
  [[nodiscard]] bool validateNode(const pNode &v) const {
    return v &&
           ((v->parent && (v->parent->left == v || v->parent->right == v)) ||
            !v->parent);
  }
#endif

public:
  // well local iterator, just looks works
  class iterator {
  public:
    //[temp.res]
    using difference_type = int;
    using value_type = T;
    using iterator_category = std::forward_iterator_tag;
    using reference = T &;
    using pointer = T *;

    pNode current;
    iterator(pNode c = nullptr) : current(c){};
    iterator &operator++();
    friend bool operator!=(const iterator &lhs, const iterator &rhs) {
      return lhs.current != rhs.current;
    }
    T operator*() const { return current->key; }
  };
  iterator begin() const;
  [[nodiscard]] iterator end() const { return iterator(nullptr); }

private:
#ifdef Debug
  [[nodiscard]] bool validateTree() const;
#endif
  // simple binary tree insert
  pNode internal_insert(T key, pNode current);
  void update_weight(pNode changed);
  pNode rotateRight(pNode x);
  pNode rotateLeft(pNode y);
  void rebalance(pNode &to_rebalance);
  void clear_node(Node *node);

public:
  void clear() { clear_node(root); }
  void insert(T key);
  [[nodiscard]] iterator lower_bound(T key) const // >= key
  {
    return std::lower_bound(begin(), end(), key);
  }
  [[nodiscard]] iterator upper_bound(T key) const // > key
  {
    return std::upper_bound(begin(), end(), key);
  }
  half_avl_tree() : root(nullptr){};
  ~half_avl_tree() { clear(); }
};
template <typename T>
typename half_avl_tree<T>::iterator half_avl_tree<T>::begin() const {
  half_avl_tree::pNode leftest = root;
  while (true) {
    if (leftest && leftest->left) {
      leftest = leftest->left;
    } else {
      break;
    }
  }
  return half_avl_tree::iterator(leftest);
}
template <typename T>
typename half_avl_tree<T>::iterator &half_avl_tree<T>::iterator::operator++() {
  if (current->right) {
    current = current->right;
    while (current->left) {
      current = current->left;
    }
    return *this;
  }
#ifdef Debug
  assert(!current->right && "logic error in iterator");
#endif
  while (current && current->parent && current == current->parent->right) {
    current = current->parent;
  }
  current = current->parent; // go from left node
  return *this;              // end is nullptr
}
#ifdef Debug
template <typename T> bool half_avl_tree<T>::validateTree() const {
  bool result = true;
  iterator it = begin();
  iterator en = end();
  while (it != en) {
    result = result && validateNode(it.current);
    if (!result) {
      break;
    }
    ++it;
  }
  return result;
}
#endif
// simple binary tree insert
template <typename T>
typename half_avl_tree<T>::pNode
half_avl_tree<T>::internal_insert(T key, half_avl_tree<T>::pNode current) {
  pNode *walk = &current; // well, need to check...
  pNode parent_walk = current->parent;
  while (*walk) {
    T walk_key = (*walk)->key;
    parent_walk = *walk;
    // if it was error all may be faster (
    if (walk_key == key) {
      return nullptr; // not error but...
    }
    if (key > walk_key) {
      walk = &(*walk)->right;
    } else {
      walk = &(*walk)->left;
    }
  }
  // now *walk == nullptr
  *walk = new Node(key); // std::make_shared<Node>(key);
  (*walk)->parent = parent_walk;
  return *walk;
}
// function called after insert new key
template <typename T>
void half_avl_tree<T>::update_weight(const half_avl_tree<T>::pNode changed) {
#ifdef Debug
  if (!validateTree()) {
    assert(false && "wrong data in update_weight");
  }
#endif
  pNode current = changed;
  while (current && current->parent) {
    if (current == current->parent->left) {
      --(current->parent->weight);
    } else if (current == current->parent->right) {
      ++(current->parent->weight);
#ifdef Debug
    } else {
      assert(false && "seems pointers wrong in update weights");
    }
#endif
    current = current->parent;
  }
}
template <typename T>
typename half_avl_tree<T>::pNode
half_avl_tree<T>::rotateRight(half_avl_tree<T>::pNode x) // wiki small right
{
#ifdef Debug
  if (!validateTree()) {
    assert(false && "wrong data in update_weight");
  }
#endif
  pNode c = x->right;
  pNode y = x->left;
  pNode a = y->left;
  pNode b = y->right;
  pNode oldParent = x->parent;
  if (oldParent && x == oldParent->left) {
    oldParent->left = y;
  } else if (oldParent && x == oldParent->right) {
    oldParent->right = y;
  }
  if (x == root) {
    root = y;
  }
  x->parent = y;
  x->right = c;
  x->left = b;

  y->parent = oldParent;
  y->right = x;
  y->left = a;

  if (a) {
    a->parent = y;
  }
  if (b) {
    b->parent = x;
  }
  if (c) {
    c->parent = x;
  }
  // update weights
  x->weight = std::abs(c ? c->weight : 0) - std::abs(b ? b->weight : 0);
  y->weight = std::abs(x->weight) - std::abs(a ? a->weight : 0);
#ifdef Debug
  if (!validateTree()) {
    assert(false && "wrong data in update_weight");
  }
#endif
  return y;
}

template <typename T>
typename half_avl_tree<T>::pNode
half_avl_tree<T>::rotateLeft(half_avl_tree<T>::pNode y) // wiki small left
{
#ifdef Debug
  if (!validateTree()) {
    assert(false && "wrong data in rotateLeft");
  }
#endif
  pNode x = y->right;
  pNode a = y->left;
  pNode b = x->left;
  pNode c = x->right;
  pNode oldParent = y->parent;
  if (oldParent && y == oldParent->left) {
    oldParent->left = x;
  } else if (oldParent && y == oldParent->right) {
    oldParent->right = x;
  }
  if (y == root) {
    root = x;
  }
  x->parent = oldParent;
  x->right = c;
  x->left = y;

  y->parent = x;
  y->left = a;
  y->right = b;
  if (a) {
    a->parent = y;
  }
  if (b) {
    b->parent = y;
  }
  if (c) {
    c->parent = x;
  }
  // update weights
  y->weight = std::abs(b ? b->weight : 0) - std::abs(a ? a->weight : 0);
  x->weight = std::abs(c ? c->weight : 0) - std::abs(y->weight);
#ifdef Debug
  if (!validateTree()) {
    bool res = validateTree();
    assert(false && "wrong data in rotateLeft");
  }
#endif
  return x;
}

template <typename T>
void half_avl_tree<T>::rebalance(half_avl_tree<T>::pNode &to_rebalance) {
#ifdef Debug
  if (!validateTree()) {
    assert(false && "wrong data in update_weight");
  }
#endif
  pNode current = to_rebalance;
  while (current) {
    int weight = current->weight;
    if (weight > 1) // right heavy
    {
      if (current->right && current->right->left) {
        // rotateRight(current->right);
        current = rotateLeft(current);
      } else {
        current = rotateLeft(current);
      }
    } else if (weight < -1) // left heavy
    {
      if (current->left && current->left->right) {
        rotateLeft(current->left);
        current = rotateRight(current);
      } else {
        current = rotateRight(current);
      }
    }
    current = current->parent;
  }
#ifdef Debug
  if (!validateTree()) {
    assert(false && "wrong data in update_weight");
  }
#endif
}

template <typename T>
void half_avl_tree<T>::clear_node(half_avl_tree<T>::pNode node) {
  if (!node) {
    return;
  }
  if (node->left) {
    clear_node(node->left);
  }
  if (node->right) {
    clear_node(node->right);
  }
  // no left & right nodes - can be deleted...
  if (!node->parent) { // it is root
    return;
  }
  if (node->parent->left == node) {
    node->parent->left = nullptr;
    delete node;
  } else if (node->parent->right == node) {
    node->parent->left = nullptr;
    delete node;
  }
#ifdef Debug
  else {
    assert(false && "delete node error: mismatch");
  }
#endif
}

template <typename T> void half_avl_tree<T>::insert(T key) {
#ifdef Debug
  if (!validateTree()) {
    assert(false && "wrong data in insert");
  }
#endif
  if (!root) {
    root = new Node(key); // std::make_shared<Node>(key);
    return;
  }
  pNode added = internal_insert(key, root);
  if (!added) {
    return;
  }
  update_weight(added);
  rebalance(added->parent);
}
}; // namespace handmade
