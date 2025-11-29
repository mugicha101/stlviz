#pragma once

#include "vstd/base.hpp"
#include "vobj/all.hpp"
#include <iostream>
#include <memory>
#include <functional>
#include <stdexcept>
#include <queue>

namespace vstd {
  template<typename T>
  class binary_tree : public base {
    using EBT = vobj::BackingType<T>::type;

    ASSERT_PRIMITIVE(T);

    // node structure
    struct Node {
      T value;
      std::shared_ptr<Node> left;
      std::shared_ptr<Node> right;
      std::shared_ptr<vobj::TreeNode<EBT>> bo; // backing object for this node

      Node(const T& val) : value(val), left(nullptr), right(nullptr), bo(nullptr) {}
    };

  public:
    // allows users to reference specific nodes
    using NodeHandle = std::shared_ptr<Node>;

  private:
    NodeHandle root;
    std::shared_ptr<vobj::Tree<EBT>> treeDisplay;

    // Recursively update all node values
    bool updateNodeValues(NodeHandle node, vobj::Operation &op) {
      if (!node) return false;

      bool ret = false;

      // Update this node's value
      if (node->bo && node->bo->element) {
        ret = ret | std::dynamic_pointer_cast<EBT>(node->bo->element)->update(op, node->value);
      }

      // Recursively update children
      ret = ret | updateNodeValues(node->left, op);
      ret = ret | updateNodeValues(node->right, op);

      return ret;
    }

    bool _vstd_update_values(vobj::Operation &op) override {
      std::cout << "BINARY_TREE UPDATE" << std::endl;
      return updateNodeValues(root, op);
    }

    void init_helper(std::source_location sloc) {
      std::cout << "BINARY_TREE INIT AT " << sloc.line() << std::endl;
      OP("binary_tree initialization",
        treeDisplay = vobj::create<vobj::Tree<EBT>>();
        treeDisplay->o = (vstd::base *)this;
        std::cout << "BO UID " << treeDisplay->uid << " MAPPED TO BINARY_TREE" << std::endl;
        op.comps.push_back(std::make_unique<vobj::ConstructOp>(treeDisplay, sloc));
      )
    }

    // Helper to create a new node with visualization backing
    NodeHandle createNode(const T& value, NodeHandle parent, bool isLeft, vobj::Operation &op) {
      // Create new internal node
      auto newNode = std::make_shared<Node>(value);

      // Create backing object for visualization
      auto nodeDisplay = vobj::create<vobj::TreeNode<EBT>>();
      auto elementDisplay = vobj::create<EBT>(value);
      nodeDisplay->element = elementDisplay;
      newNode->bo = nodeDisplay;

      // Find parent's backing object
      std::shared_ptr<vobj::TreeNode<EBT>> parentDisplay = nullptr;
      if (parent) {
        parentDisplay = parent->bo;
      }

      // Add to tree display (this will set node alive and parent)
      treeDisplay->addNode(op, parentDisplay, nodeDisplay, isLeft);

      // Set element as child of node display and mark it alive
      elementDisplay->parent = nodeDisplay;
      elementDisplay->setAlive(true);

      // Set initial value
      op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(elementDisplay, elementDisplay->value, value));

      return newNode;
    }

    // Helper to remove a node and its subtree
    void removeNodeHelper(NodeHandle node, vobj::Operation &op) {
      if (!node) return;

      // Recursively remove children first
      removeNodeHelper(node->left, op);
      removeNodeHelper(node->right, op);

      // Remove this node's visualization
      if (node->bo) {
        if (node->bo->element) {
          node->bo->element->setAlive(false);
        }
        node->bo->setAlive(false);
      }
    }

  public:
    void _vstd_rename(std::string name, SLOC) override {
      OP("rename binary_tree",
        op.comps.push_back(std::make_unique<vobj::RenameOp>(treeDisplay, treeDisplay->name, name));
      )
    }

    std::string _vstd_type_name() const override {
      return "binary_tree";
    }

    // Constructor
    binary_tree(SLOC) : root(nullptr) {
      init_helper(sloc);
    }

    // Destructor
    ~binary_tree() {
      SLOC;
      OP("binary_tree destruction",
        // Clear all nodes
        treeDisplay->clear(op);
        root = nullptr;
        treeDisplay->o = nullptr;
        op.comps.push_back(std::make_unique<vobj::DestroyOp>(treeDisplay));
      )
    }

    // Check if tree is empty
    bool empty() const {
      return root == nullptr;
    }

    // Get the root node handle
    NodeHandle getRoot() const {
      return root;
    }

    // Set the root value (creates root if it doesn't exist)
    void setRoot(const T& value, SLOC) {
      OP("binary_tree set root",
        if (root) {
          // Update existing root
          root->value = value;
          if (root->bo && root->bo->element) {
            auto elem = std::dynamic_pointer_cast<EBT>(root->bo->element);
            op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(elem, elem->latest, value));
          }
        } else {
          // Create new root
          root = createNode(value, nullptr, false, op);
        }
      )
    }

    // Insert a left child at the specified parent node
    // Returns the newly created node handle
    NodeHandle insertLeft(NodeHandle parent, const T& value, SLOC) {
      if (!parent) {
        throw std::runtime_error("Cannot insert left child: parent node is null");
      }

      NodeHandle result = nullptr;
      OP("binary_tree insert left",
        if (parent->left) {
          throw std::runtime_error("Cannot insert left child: left child already exists");
        }
        parent->left = createNode(value, parent, true, op);
        result = parent->left;
      )
      return result;
    }

    // Insert a right child at the specified parent node
    // Returns the newly created node handle
    NodeHandle insertRight(NodeHandle parent, const T& value, SLOC) {
      if (!parent) {
        throw std::runtime_error("Cannot insert right child: parent node is null");
      }

      NodeHandle result = nullptr;
      OP("binary_tree insert right",
        if (parent->right) {
          throw std::runtime_error("Cannot insert right child: right child already exists");
        }
        parent->right = createNode(value, parent, false, op);
        result = parent->right;
      )
      return result;
    }

    // Get the left child of a node
    NodeHandle getLeftChild(NodeHandle node) const {
      return node ? node->left : nullptr;
    }

    // Get the right child of a node
    NodeHandle getRightChild(NodeHandle node) const {
      return node ? node->right : nullptr;
    }

    // Get the value of a node
    const T& getValue(NodeHandle node) const {
      if (!node) {
        throw std::runtime_error("Cannot get value: node is null");
      }
      return node->value;
    }

    // Set the value of a node
    void setValue(NodeHandle node, const T& value, SLOC) {
      if (!node) {
        throw std::runtime_error("Cannot set value: node is null");
      }

      OP("binary_tree set value",
        node->value = value;
        if (node->bo && node->bo->element) {
          auto elem = std::dynamic_pointer_cast<EBT>(node->bo->element);
          op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(elem, elem->latest, value));
        }
      )
    }

    // Remove a node and its entire subtree
    void removeSubtree(NodeHandle parent, bool isLeft, SLOC) {
      if (!parent) {
        throw std::runtime_error("Cannot remove subtree: parent node is null");
      }

      OP("binary_tree remove subtree",
        NodeHandle& childRef = isLeft ? parent->left : parent->right;
        if (!childRef) {
          throw std::runtime_error("Cannot remove subtree: child does not exist");
        }

        std::shared_ptr<vobj::TreeNode<EBT>> parentDisplay = parent->bo;
        std::shared_ptr<vobj::TreeNode<EBT>> childDisplay = childRef->bo;

        // Remove child and its subtree from visualization
        treeDisplay->removeNode(op, parentDisplay, childDisplay, isLeft);

        // Clear the subtree
        removeNodeHelper(childRef, op);
        childRef = nullptr;
      )
    }

    // Clear the entire tree
    void clear(SLOC) {
      OP("binary_tree clear",
        if (root) {
          removeNodeHelper(root, op);
          root = nullptr;
          treeDisplay->root = nullptr;
        }
      )
    }

    // Find a value in the tree (linear search through all nodes)
    NodeHandle find(const T& value) const {
      if (!root) return nullptr;

      // BFS to find the value
      std::queue<NodeHandle> q;
      q.push(root);

      while (!q.empty()) {
        auto node = q.front();
        q.pop();

        if (node->value == value) {
          return node;
        }

        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
      }

      return nullptr;
    }

    // Check if a value exists in the tree
    bool contains(const T& value) const {
      return find(value) != nullptr;
    }

    // Get the height of the tree
    int height() const {
      return heightHelper(root);
    }

  private:
    int heightHelper(NodeHandle node) const {
      if (!node) return 0;
      return 1 + std::max(heightHelper(node->left), heightHelper(node->right));
    }

  public:
    // Count total number of nodes in the tree
    int size() const {
      return sizeHelper(root);
    }

  private:
    int sizeHelper(NodeHandle node) const {
      if (!node) return 0;
      return 1 + sizeHelper(node->left) + sizeHelper(node->right);
    }
  };
}
