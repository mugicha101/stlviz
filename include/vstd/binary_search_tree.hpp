#pragma once

#include "vstd/base.hpp"
#include "vobj/all.hpp"
#include <iostream>
#include <memory>
#include <functional>

namespace vstd {
  template<typename T>
  class binary_search_tree : public base {
    using EBT = vobj::BackingType<T>::type;

    // assert that element is a primitive
    ASSERT_PRIMITIVE(T);

    // Internal node structure
    struct Node {
      T value;
      std::shared_ptr<Node> left;
      std::shared_ptr<Node> right;
      std::shared_ptr<vobj::TreeNode<EBT>> bo; // backing object for this node

      Node(const T& val) : value(val), left(nullptr), right(nullptr), bo(nullptr) {}
    };

    std::shared_ptr<Node> root;
    std::shared_ptr<vobj::Tree<EBT>> treeDisplay;

    // Recursively update all node values
    bool updateNodeValues(std::shared_ptr<Node> node, vobj::Operation &op) {
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
      std::cout << "BINARY_SEARCH_TREE UPDATE" << std::endl;
      return updateNodeValues(root, op);
    }

    void init_helper(std::source_location sloc) {
      std::cout << "BINARY_SEARCH_TREE INIT AT " << sloc.line() << std::endl;
      OP("binary_search_tree initialization",
        treeDisplay = vobj::create<vobj::Tree<EBT>>();
        treeDisplay->o = (vstd::base *)this;
        std::cout << "BO UID " << treeDisplay->uid << " MAPPED TO BINARY_SEARCH_TREE" << std::endl;
        op.comps.push_back(std::make_unique<vobj::ConstructOp>(treeDisplay, sloc));
      )
    }

    // Helper function to insert a node in BST order
    std::shared_ptr<Node> insertHelper(std::shared_ptr<Node> node, const T& value,
                                       std::shared_ptr<Node> parent, bool isLeft,
                                       vobj::Operation &op) {
      if (!node) {
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

      if (value < node->value) {
        node->left = insertHelper(node->left, value, node, true, op);
      } else if (value > node->value) {
        node->right = insertHelper(node->right, value, node, false, op);
      }
      // If value == node->value, we don't insert (BST property)

      return node;
    }

    // Helper to find minimum value node in subtree
    std::shared_ptr<Node> findMin(std::shared_ptr<Node> node) {
      while (node && node->left) {
        node = node->left;
      }
      return node;
    }

    // Helper function to delete a node in BST
    std::shared_ptr<Node> deleteHelper(std::shared_ptr<Node> node, const T& value,
                                       std::shared_ptr<Node> parent, bool isLeft,
                                       vobj::Operation &op) {
      if (!node) {
        return nullptr;
      }

      if (value < node->value) {
        node->left = deleteHelper(node->left, value, node, true, op);
      } else if (value > node->value) {
        node->right = deleteHelper(node->right, value, node, false, op);
      } else {
        // Found the node to delete
        std::shared_ptr<vobj::TreeNode<EBT>> parentDisplay = parent ? parent->bo : nullptr;

        // Case 1: Leaf node or node with one child
        if (!node->left) {
          auto temp = node->right;
          treeDisplay->removeNode(op, parentDisplay, node->bo, isLeft);
          return temp;
        } else if (!node->right) {
          auto temp = node->left;
          treeDisplay->removeNode(op, parentDisplay, node->bo, isLeft);
          return temp;
        }

        // Case 2: Node with two children
        // Find successor (inorder)
        auto successor = findMin(node->right);

        // Copy successor's value to this node
        node->value = successor->value;
        if (node->bo && node->bo->element) {
          auto elem = std::dynamic_pointer_cast<EBT>(node->bo->element);
          op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(elem, elem->latest, successor->value));
        }

        // Delete the successor
        node->right = deleteHelper(node->right, successor->value, node, false, op);
      }

      return node;
    }

  public:
    void _vstd_rename(std::string name, SLOC) override {
      OP("rename binary_search_tree",
        op.comps.push_back(std::make_unique<vobj::RenameOp>(treeDisplay, treeDisplay->name, name));
      )
    }

    std::string _vstd_type_name() const override {
      return "binary_search_tree";
    }

    // Constructor
    binary_search_tree(SLOC) : root(nullptr) {
      init_helper(sloc);
    }

    // Destructor
    ~binary_search_tree() {
      SLOC;
      OP("binary_search_tree destruction",
        // Clear all nodes
        treeDisplay->clear(op);
        root = nullptr;
        op.comps.push_back(std::make_unique<vobj::DestroyOp>(treeDisplay));
      )
    }

    // Insert a value into the BST
    void insert(const T& value, SLOC) {
      OP("binary_search_tree insert",
        root = insertHelper(root, value, nullptr, false, op);
      )
    }

    // Remove a value from the BST
    void remove(const T& value, SLOC) {
      OP("binary_search_tree remove",
        root = deleteHelper(root, value, nullptr, false, op);
      )
    }

    // Check if tree is empty
    bool empty() const {
      return root == nullptr;
    }

    // Find a value in the tree
    bool find(const T& value) const {
      auto node = root;
      while (node) {
        if (value == node->value) return true;
        else if (value < node->value) node = node->left;
        else node = node->right;
      }
      return false;
    }

    // Get the root value (throws if empty)
    const T& getRootValue() const {
      if (!root) {
        throw std::runtime_error("Tree is empty");
      }
      return root->value;
    }
  };
}
