#include <stlviz.hpp>
#include <numeric>

// debug program

int main() {
  // vstd::vector<int> vec(5, 0);
  // DEF(vec);
  // vec[0] = 5;
  // vec[1] = 3;
  // vec[2] = 8;
  // vec[3] = 1;
  // vec[4] = 9;

  // vstd::vector<int> vec2(20);
  // for (int i = 0; i < vec2.size(); ++i) {
  //   vec2[i] = rand() % 100;
  // }
  // vec2.push_back(0);
  // sort(vec2.begin(), vec2.end());

  // // Stack demonstration
  // vstd::stack<int> s;
  // DEF(s);
  // s.push(10);
  // s.push(20);
  // s.push(30);
  // s.push(40);
  // s.pop();
  // s.pop();
  // s.push(50);


  // for (int i = 0; i < 10; ++i) {
  //   if (!vec.empty()) vec[rand() % vec.size()] = rand() % 100;
  //   if (vec.empty() || rand() % 2) vec.push_back(rand() % 100);
  //   else vec.pop_back();
  // }

  // // for (int i = 0; i < 1000; ++i) vec.push_back(i);

  // vstd::deque<int> dq;
  // DEF(dq);
  // dq.push_back(4);
  // dq.push_back(20);
  // dq.push_front(10);
  // dq.pop_back();
  // dq.pop_front();
  // dq.pop_back();

  vstd::binary_search_tree<int> bst;
  DEF(bst);
  bst.insert(1);
  bst.insert(2);
  bst.insert(3);
  bst.insert(4);
  bst.insert(5);
  bst.insert(6);

  // binary tree 
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

  _Exit(0);
}
