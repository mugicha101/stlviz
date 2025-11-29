#include <stlviz.hpp>

int dfs(vstd::binary_tree<int>::NodeHandle node) {
  // max max path in subtrees and get subtree chains (path from node to leaf)
  vstd::Int value = node->value;
  DEF(value);
  vstd::Int maxPathSum = 0;
  DEF(maxPathSum);
  vstd::Int leftChain = 0;
  DEF(leftChain);
  if (node->left) {
    leftChain = dfs(node->left);
    maxPathSum = std::max((int)maxPathSum, node->left->value);
  }
  vstd::Int rightChain = 0;
  DEF(rightChain);
  if (node->right) {
    rightChain = dfs(node->left);
    maxPathSum = std::max((int)maxPathSum, node->right->value);
  }

  // candidate path: leftChain -> node -> rightChain
  maxPathSum = std::max((int)maxPathSum, leftChain + rightChain);
  node->value = maxPathSum;
  SNAP;

  // returns max path sum from node to leaf
  return std::max(leftChain, rightChain) + value;
}

int main() {
  // INPUT
  vstd::binary_tree<int> tree;
  {
    tree.setRoot(-10);
    tree.insertLeft(tree.getRoot(), 9);
    auto r = tree.insertRight(tree.getRoot(), 20);
    tree.insertLeft(r, 15);
    tree.insertRight(r, 7);
  }

  // ALGORITHM
  dfs(tree.getRoot());

  // OUTPUT - check tree
  MAIN_DONE;
}
