#include "avl_tree.hpp"
#include "point.hpp"
#include <iostream>

using namespace std;

template <typename T>
void print_search(const AvlTree<T>& tree, const T& value) {
  if (tree.search(value)) cout << "Found " << value << '\n';
  else cout << value << " not found\n";
}

int main() {
  cout << "=== Insert Test ===\n";

  AvlTree<int> tree;

  const int values[] = {
    100, 20, 200, 10, 30, 150, 300, 25, 40, 50, 5, 17, 60, 12, 33, 45, 22
  };

  for (int value : values) tree.insert(value);

  cout << "\n=== Traversal Test ===\n";

  cout << "Preorder: ";
  tree.print_preorder();

  cout << "Postorder: ";
  tree.print_postorder();

  cout << "Inorder: ";
  tree.print_inorder();

  cout << "\n=== Search Test ===\n";

  print_search(tree, 100);
  print_search(tree, 17);
  print_search(tree, 999);
  print_search(tree, -10);

  cout << "\n=== Pretty Print Test ===\n";

  tree.pretty_print();

  cout << "\n=== Clone Test ===\n";

  AvlTree<int> clone = tree.clone();

  cout << "Original:\n";
  tree.pretty_print();

  cout << "Clone:\n";
  clone.pretty_print();

  cout << "\nInsert into clone only:\n";

  clone.insert(500);

  cout << "Original after clone modification:\n";
  tree.pretty_print();

  cout << "Clone after modification:\n";
  clone.pretty_print();

  cout << "\n=== Move Constructor Test ===\n";

  AvlTree<int> moved = move(tree);

  cout << "Moved tree:\n";
  moved.pretty_print();

  cout << "Original after move:\n";
  tree.pretty_print();

  cout << "\n=== Move Assignment Test ===\n";

  AvlTree<int> assigned;

  assigned = move(clone);

  cout << "Assigned tree:\n";
  assigned.pretty_print();

  cout << "Clone after move:\n";
  clone.pretty_print();

  cout << "\n=== Clear Test ===\n";

  assigned.clear();

  cout << "After clear:\n";
  assigned.pretty_print();

  cout << "\n=== Destructor Test ===\n";

  {
    AvlTree<int> temporary;

    temporary.insert(10);
    temporary.insert(20);
    temporary.insert(30);

    cout << "Temporary tree:\n";
    temporary.pretty_print();
  }

  cout << "Temporary destroyed successfully\n";

  cout << "\n=== AVL Rotation Tests ===\n";

  cout << "\nLL Case (Right Rotation):\n";

  AvlTree<int> ll;

  ll.insert(30);
  ll.insert(20);
  ll.insert(10);

  ll.pretty_print();

  cout << "\nRR Case (Left Rotation):\n";

  AvlTree<int> rr;

  rr.insert(10);
  rr.insert(20);
  rr.insert(30);

  rr.pretty_print();

  cout << "\nLR Case (Left-Right Rotation):\n";

  AvlTree<int> lr;

  lr.insert(30);
  lr.insert(10);
  lr.insert(20);

  lr.pretty_print();

  cout << "\nRL Case (Right-Left Rotation):\n";

  AvlTree<int> rl;

  rl.insert(10);
  rl.insert(30);
  rl.insert(20);

  rl.pretty_print();

  cout << "\n=== Duplicate Insert Test ===\n";

  AvlTree<int> duplicate;

  duplicate.insert(10);
  duplicate.insert(20);
  duplicate.insert(10);
  duplicate.insert(20);
  duplicate.insert(30);

  cout << "Inorder after duplicates: ";
  duplicate.print_inorder();

  cout << "\n=== Empty Tree Test ===\n";

  AvlTree<int> empty;

  cout << "Search empty tree: ";
  print_search(empty, 10);

  cout << "Pretty print empty tree:\n";
  empty.pretty_print();

  cout << "\n=== Genericity Test (AvlTree<std::string>) ===\n";

  AvlTree<string> words;

  for (const string& w : {
    "pear", "apple", "mango", "kiwi", "grape", "fig", "date"
  }) words.insert(w);

  cout << "Inorder: ";
  words.print_inorder();

  print_search(words, string("mango"));
  print_search(words, string("banana"));

  words.pretty_print();

  cout << "\n=== Genericity Test (AvlTree<float>) ===\n";

  AvlTree<float> floats;

  for (float f : {3.5f, 1.25f, 9.0f, 4.75f, 2.0f}) floats.insert(f);

  cout << "Inorder: ";
  floats.print_inorder();

  print_search(floats, 4.75f);
  print_search(floats, 100.0f);

  floats.pretty_print();

  cout << "\n=== Genericity Test (AvlTree<Point>, a custom struct) ===\n";

  AvlTree<Point> points;

  points.insert(Point(3, 1));
  points.insert(Point(1, 5));
  points.insert(Point(7, 2));
  points.insert(Point(2, 0));

  cout << "Inorder: ";
  points.print_inorder();

  print_search(points, Point(1, 5));
  print_search(points, Point(9, 9));

  points.pretty_print();

  return 0;
}
