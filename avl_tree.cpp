#include "avl_tree.hpp"
#include "point.hpp"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename T> struct Node {
  T value;
  Node *left = nullptr;
  Node *right = nullptr;
  int height = 0;

  explicit Node(const T &value) : value(value) {}
};

namespace {

template <typename T> int get_height(const Node<T> *node) {
  return node == nullptr ? -1 : node->height;
}

template <typename T> void compute_height(Node<T> *node) {
  node->height = 1 + std::max(get_height(node->left), get_height(node->right));
}

template <typename T> int compute_balance_factor(const Node<T> *node) {
  return get_height(node->left) - get_height(node->right);
}

// Promotes node->*heavy to be the new subtree root, in place. Both
// left_rotate and right_rotate are this same shape mirrored, so rather than
// duplicating the logic, the child to promote (and where its other child
// lands) are passed as pointers-to-member:
//   right_rotate(node) == rotate(node, &Node<T>::left,  &Node<T>::right)
//   left_rotate(node)  == rotate(node, &Node<T>::right, &Node<T>::left)
template <typename T>
void rotate(
  Node<T> *&node, Node<T> *Node<T>::*heavy, Node<T> *Node<T>::*other
) {
  Node<T> *new_root = node->*heavy;

  node->*heavy = new_root->*other;
  new_root->*other = node;

  compute_height(node);
  compute_height(new_root);

  node = new_root;
}

template <typename T> void balance(Node<T> *&node) {
  if (node == nullptr) return;

  compute_height(node);

  int bf = compute_balance_factor(node);

  if (bf > 1) {
    if (compute_balance_factor(node->left) < 0)
      rotate(node->left, &Node<T>::right, &Node<T>::left);
    rotate(node, &Node<T>::left, &Node<T>::right);
  } else if (bf < -1) {
    if (compute_balance_factor(node->right) > 0)
      rotate(node->right, &Node<T>::left, &Node<T>::right);
    rotate(node, &Node<T>::right, &Node<T>::left);
  }
}

template <typename T> void insert_impl(Node<T> *&root, const T &value) {
  if (root == nullptr) {
    root = new Node<T>(value);
    return;
  }

  // Only operator< is required of T, so "greater than" is expressed as
  // root->value < value rather than value > root->value.
  if (value < root->value)
    insert_impl(root->left, value);
  else if (root->value < value)
    insert_impl(root->right, value);
  else
    return;

  balance(root);
}

template <typename T> bool search_impl(const Node<T> *root, const T &value) {
  while (root != nullptr) {
    if (value == root->value) return true;

    if (value < root->value)
      root = root->left;
    else
      root = root->right;
  }

  return false;
}

template <typename T> void print_preorder_impl(const Node<T> *root) {
  if (root == nullptr) return;

  std::cout << root->value << ' ';
  print_preorder_impl(root->left);
  print_preorder_impl(root->right);
}

template <typename T> void print_postorder_impl(const Node<T> *root) {
  if (root == nullptr) return;

  print_postorder_impl(root->left);
  print_postorder_impl(root->right);
  std::cout << root->value << ' ';
}

template <typename T> void print_inorder_impl(const Node<T> *root) {
  if (root == nullptr)
    return;

  print_inorder_impl(root->left);
  std::cout << root->value << ' ';
  print_inorder_impl(root->right);
}

template <typename T>
void fill_grid(
  const Node<T> *node,
  int depth,
  int col_left,
  int col_right,
  std::vector<std::string> &rows
) {
  if (node == nullptr) return;

  int mid = (col_left + col_right) / 2;
  int row = depth * 2;

  // std::to_string only works for arithmetic types, so use an ostream
  // (via operator<<) to support any printable T.
  std::ostringstream oss;
  oss << node->value;
  std::string value = oss.str();

  int start = mid - static_cast<int>(value.size()) / 2;
  start = std::max(
    0, std::min(
      start,
      static_cast<int>(rows[row].size()) - static_cast<int>(value.size())
    )
  );

  for (size_t i = 0; i < value.size(); ++i)
    rows[row][start + i] = value[i];

  if (node->left != nullptr) {
    int child_mid = (col_left + mid) / 2;

    rows[row + 1][child_mid + 1] = '/';

    fill_grid(node->left, depth + 1, col_left, mid, rows);
  }

  if (node->right != nullptr) {
    int child_mid = (mid + col_right) / 2;

    rows[row + 1][child_mid - 1] = '\\';

    fill_grid(node->right, depth + 1, mid, col_right, rows);
  }
}

template <typename T> void pretty_print_impl(const Node<T> *root) {
  if (root == nullptr)
    return;

  int height = get_height(root);
  int width = 1 << (height + 2);
  int rows_count = height * 2 + 1;

  std::vector<std::string> rows(rows_count, std::string(width, ' '));

  fill_grid(root, 0, 0, width, rows);

  for (const auto &row : rows) {
    size_t end = row.find_last_not_of(' ');

    if (end == std::string::npos)
      std::cout << '\n';
    else
      std::cout << row.substr(0, end + 1) << '\n';
  }
}

template <typename T> void destroy(Node<T> *node) {
  if (node == nullptr)
    return;

  destroy(node->left);
  destroy(node->right);

  delete node;
}

template <typename T> Node<T> *clone_node(const Node<T> *node) {
  if (node == nullptr)
    return nullptr;

  Node<T> *copy = new Node<T>(node->value);

  copy->height = node->height;
  copy->left = clone_node(node->left);
  copy->right = clone_node(node->right);

  return copy;
}

} // namespace

template <typename T> AvlTree<T>::~AvlTree() { clear(); }

template <typename T> void AvlTree<T>::insert(const T &value) {
  ::insert_impl(root, value);
}

template <typename T> bool AvlTree<T>::search(const T &value) const {
  return ::search_impl(root, value);
}

template <typename T> void AvlTree<T>::print_preorder() const {
  ::print_preorder_impl(root);
  std::cout << '\n';
}

template <typename T> void AvlTree<T>::print_postorder() const {
  ::print_postorder_impl(root);
  std::cout << '\n';
}

template <typename T> void AvlTree<T>::print_inorder() const {
  ::print_inorder_impl(root);
  std::cout << '\n';
}

template <typename T> void AvlTree<T>::pretty_print() const {
  ::pretty_print_impl(root);
}

template <typename T> void AvlTree<T>::clear() {
  destroy(root);
  root = nullptr;
}

template <typename T> AvlTree<T>::AvlTree(AvlTree &&other) noexcept {
  root = other.root;
  other.root = nullptr;
}

template <typename T>
AvlTree<T> &AvlTree<T>::operator=(AvlTree &&other) noexcept {
  if (this != &other) {
    clear();

    root = other.root;
    other.root = nullptr;
  }

  return *this;
}

template <typename T> AvlTree<T> AvlTree<T>::clone() const {
  AvlTree copy;

  copy.root = ::clone_node(root);

  return copy;
}

// ---------------------------------------------------------------------
// Explicit template instantiations.
//
// Because Node<T> and every AvlTree<T> member function are defined in
// this .cpp file (not in the header), the compiler can only generate
// code for a T here, where it can see the definitions. Any T used in
// another translation unit MUST have a corresponding line below, or
// you'll get "undefined reference to AvlTree<T>::..." linker errors.
// ---------------------------------------------------------------------

template class AvlTree<int>;
template class AvlTree<std::string>;
template class AvlTree<float>;
template class AvlTree<Point>;
// Add more instantiations here as needed, e.g.:
// template class AvlTree<double>;
// template class AvlTree<MyType>;
