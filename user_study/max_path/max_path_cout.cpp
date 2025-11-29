#include <vector>
#include <iostream>
#include <queue>

struct TreeNode {
  TreeNode *left = nullptr;
  TreeNode *right = nullptr;
  int value;

  TreeNode(int value) : value(value) {}
};

int dfs(TreeNode *node) {
  // max max path in subtrees and get subtree chains (path from node to leaf)
  int value = node->value;
  int maxPathSum = 0;
  int leftChain = 0;
  if (node->left) {
    leftChain = dfs(node->left);
    maxPathSum = std::max(maxPathSum, node->left->value);
  }
  int rightChain = 0;
  if (node->right) {
    rightChain = dfs(node->right);
    maxPathSum = std::max(maxPathSum, node->right->value);
  }

  // candidate path: leftChain -> node -> rightChain
  maxPathSum = std::max(maxPathSum, leftChain + rightChain);
  node->value = maxPathSum;

  // returns max path sum from node to leaf
  return std::max(leftChain, rightChain) + node->value;
}

int main() {
  // INPUT
  TreeNode *tree = new TreeNode(-10);
  {
    auto l = tree->left = new TreeNode(9);
    auto r = tree->right = new TreeNode(20);
    r->left = new TreeNode(15);
    r->right = new TreeNode(7);
  }

  // ALGORITHM
  int output = 0;
  dfs(tree);

  // OUTPUT - preorder traversal
  std::queue<TreeNode *> q;
  q.push(tree);
  std::cout << "tree (preorder): [ ";
  while (!q.empty()) {
    TreeNode *curr = q.front();
    q.pop();
    std::cout << curr->value << " ";
    if (curr->left) q.push(curr->left);
    if (curr->right) q.push(curr->right);
  }
  std::cout << "]" << std::endl;
}
