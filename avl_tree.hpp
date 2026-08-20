// avl_tree.hpp
#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

// AvlTree<T> requires:
//   - T has operator< (used for ordering/searching)
//   - T has operator<< to an ostream (used by the printing functions)
//   - T is copy-constructible
//
// IMPORTANT: Node<T> and every AvlTree<T> member function are defined in
// avl_tree.cpp, not here. Because of that, avl_tree.cpp must explicitly
// instantiate AvlTree<T> for every T you intend to use (see the bottom
// of that file). Using AvlTree<SomeOtherType> without adding an explicit
// instantiation for it will fail to link.

template <typename T>
struct Node;

template <typename T>
class AvlTree {
public:
  AvlTree() = default;
  ~AvlTree();

  void insert(const T& value);
  bool search(const T& value) const;
  void print_preorder() const;
  void print_postorder() const;
  void print_inorder() const;
  void pretty_print() const;
  void clear();

  // Prevent shallow copies that would cause multiple trees to own the same nodes.
  AvlTree(const AvlTree&) = delete;
  AvlTree& operator=(const AvlTree&) = delete;

  // Move ownership of nodes instead of copying them.
  AvlTree(AvlTree&& other) noexcept;
  AvlTree& operator=(AvlTree&& other) noexcept;

  // Creates an independent copy of the tree.
  AvlTree clone() const;

private:
  Node<T>* root = nullptr;
};

#endif
