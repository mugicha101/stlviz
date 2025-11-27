#include <stlviz.hpp>
#include <numeric>

// debug program

// int dfs(vstd::binary_tree<int>::NodeHandle node, vstd::Int& max_path) {
//   // node is common ancestor of path
//   vstd::Int left = node->left? dfs(node->left, max_path) : 0; // max line starting from node->left
//   vstd::Int right = node->right? dfs(node->right, max_path) : 0; // max line starting from node->right
//   max_path = std::max((int)max_path, left + right + node->value); // left + right + node->val is max path with node as common ancestor
//   return std::max(std::max(left, right) + node->value, 0); // return max line starting from node
// }

int main() {
  // vstd::binary_tree<int> tree;
  // tree.setRoot(-10);
  // vstd::binary_tree<int>::NodeHandle root = tree.getRoot();
  // tree.insertLeft(root, 9);
  // auto node = tree.insertRight(root, 20);
  // tree.insertLeft(node, 15);
  // node = tree.insertRight(node, 7);
  // vstd::Int maxPath = 0;
  // dfs(root, maxPath);

  // vstd::vector<vstd::vector<int>> vec2d(10);

  // return 0;

  // Bitset demonstration
  vstd::bitset<8> a, b, c;
  DEF(a);
  DEF(b);
  DEF(c);

  a.set(0);
  a.set(2);
  a.set(4);
  b.set(1);
  b.set(2);
  b.set(3);

  for (std::size_t i = 0; i < c.size(); ++i) {
    c.set(i, a.test(i) && b.test(i));
  }

  // Set demonstration
  vstd::set<int> s;
  DEF(s);
  s.insert(5);
  s.insert(1);
  s.insert(3);
  s.erase(1);
  s.clear();

  // Vector demonstrations
  vstd::vector<int> vec(5, 0);
  DEF(vec);
  vec[0] = 5;
  vec[1] = 3;
  vec[2] = 8;
  vec[3] = 1;
  vec[4] = 9;

  vstd::vector<int> vec2(20);
  for (int i = 0; i < vec2.size(); ++i) {
    vec2[i] = rand() % 100;
  }
  vec2.push_back(0);
  vstd::quick_sort(vec2.begin(), vec2.end());

  // Stack demonstration
  vstd::stack<int> stk;
  DEF(stk);
  stk.push(10);
  stk.push(20);
  stk.push(30);
  stk.push(40);
  stk.pop();
  stk.pop();
  stk.push(50);

  for (int i = 0; i < 10; ++i) {
    if (!vec.empty()) vec[rand() % vec.size()] = rand() % 100;
    if (vec.empty() || rand() % 2) vec.push_back(rand() % 100);
    else vec.pop_back();
  }

  // Deque demonstration
  vstd::deque<int> dq;
  DEF(dq);
  dq.push_back(4);
  dq.push_back(20);
  dq.push_front(10);
  dq.pop_back();
  dq.pop_front();
  dq.pop_back();

  vstd::Int ia = 5;
  DEF(ia);
  vstd::Int ib = ia;
  DEF(ib);
  ia = 10;
  ia += 10;
  vstd::Int ic = std::move(ia);
  ia = 3;
  DEF(ic);
  ic = ib * 2 + 15;
  ++ia;
  ic = ia++;
  ic += ia;

  // Binary search tree demonstration
  vstd::binary_search_tree<int> bst;
  DEF(bst);
  bst.insert(1);
  bst.insert(2);
  bst.insert(3);
  bst.insert(4);
  bst.insert(5);
  bst.insert(6);

  // Binary tree demonstration
  vstd::binary_tree<int> bt;
  DEF(bt);
  bt.setRoot(1);
  vstd::binary_tree<int>::NodeHandle root = bt.getRoot();
  vstd::binary_tree<int>::NodeHandle node2 = bt.insertLeft(root, 2);
  vstd::binary_tree<int>::NodeHandle node3 = bt.insertRight(root, 3);
  vstd::binary_tree<int>::NodeHandle node4 = bt.insertLeft(node2, 4);
  vstd::binary_tree<int>::NodeHandle node5 = bt.insertRight(node2, 5);
  bt.insertLeft(node4, 6);

  MAIN_DONE();

  // _Exit(0);
}
