#pragma once

#include "vobj/display.hpp"
#include "vobj/backing_type.hpp"
#include <memory>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <queue>

namespace vobj {
  template<typename T>
  struct TreeNode : public Display {
    std::shared_ptr<T> element; // the value being displayed
    std::shared_ptr<TreeNode<T>> leftChild;
    std::shared_ptr<TreeNode<T>> rightChild;

    // cached layout information
    float subtreeWidth = 0.f;
    float xOffset = 0.f; // relative to parent
    float yOffset = 0.f;

  protected:
    TreeNode() : Display() {
      element = nullptr;
      leftChild = nullptr;
      rightChild = nullptr;
    }

    FRIEND_CREATE

  public:
    void draw() override {
      const int nodePadding = 10;
      const int nodeBorder = 3;

      if (!element) {
        resetCanvas(50, 50);
        canvas.display();
        return;
      }

      auto ebox = element->getBBox();
      int width = ebox.size.x + nodePadding * 2 + nodeBorder * 2;
      int height = ebox.size.y + nodePadding * 2 + nodeBorder * 2;

      // Draw circular background first (so text appears on top)
      float radius = (float)std::max(width, height) / 2.f;

      // Ensure canvas is large enough for the circle
      int canvasWidth = std::max(width, (int)(2 * radius));
      int canvasHeight = std::max(height, (int)(2 * radius));

      resetCanvas(canvasWidth, canvasHeight);

      sf::CircleShape circle(radius);
      circle.setOutlineThickness(-(float)nodeBorder);
      circle.setOutlineColor(sf::Color::Black);
      circle.setFillColor(sf::Color::White);
      circle.setOrigin({radius, radius});
      circle.setPosition({(float)canvasWidth / 2.f, (float)canvasHeight / 2.f});
      canvas.draw(circle);

      // Draw element on top of circle (centered)
      float elementX = (canvasWidth - ebox.size.x) / 2.f;
      float elementY = (canvasHeight - ebox.size.y) / 2.f;
      element->drawOn(canvas, elementX, elementY, shared_from_this());

      canvas.display();
    }

    // Calculate the width this subtree needs
    float calculateSubtreeWidth(float nodeSpacing) {
      if (!leftChild && !rightChild) {
        subtreeWidth = (float)getBBox().size.x;
        return subtreeWidth;
      }

      float leftWidth = 0.f;
      float rightWidth = 0.f;

      if (leftChild && leftChild->isAlive()) {
        leftWidth = leftChild->calculateSubtreeWidth(nodeSpacing);
      }
      if (rightChild && rightChild->isAlive()) {
        rightWidth = rightChild->calculateSubtreeWidth(nodeSpacing);
      }

      subtreeWidth = std::max((float)getBBox().size.x, leftWidth + rightWidth + nodeSpacing);
      return subtreeWidth;
    }

    // Set position relative to parent
    void layoutSubtree(float x, float y, float nodeSpacing, float levelSpacing) {
      // x is the center position, but xOffset is the left edge
      float myWidth = (float)getBBox().size.x;
      xOffset = x - myWidth / 2.f;
      yOffset = y;

      if (!leftChild && !rightChild) {
        return;
      }

      float leftWidth = 0.f;
      float rightWidth = 0.f;

      if (leftChild && leftChild->isAlive()) {
        leftWidth = leftChild->subtreeWidth;
      }
      if (rightChild && rightChild->isAlive()) {
        rightWidth = rightChild->subtreeWidth;
      }

      // Calculate child positions
      float leftX = x - (leftWidth + nodeSpacing) / 2.f;
      float rightX = x + (rightWidth + nodeSpacing) / 2.f;
      float childY = y + levelSpacing;

      if (leftChild && leftChild->isAlive()) {
        leftChild->layoutSubtree(leftX, childY, nodeSpacing, levelSpacing);
      }
      if (rightChild && rightChild->isAlive()) {
        rightChild->layoutSubtree(rightX, childY, nodeSpacing, levelSpacing);
      }
    }
  };

  template<typename T>
  struct Tree : public Display {
    std::shared_ptr<TreeNode<T>> root;

    // AddNodeOp - adds a node as child of parent
    struct AddNodeOp : public OpComp {
      std::shared_ptr<Tree<T>> tree;
      std::shared_ptr<TreeNode<T>> parent; // nullptr if root
      std::shared_ptr<TreeNode<T>> node;
      bool isLeftChild; // true = left, false = right

      AddNodeOp(std::shared_ptr<Tree<T>> tree, std::shared_ptr<TreeNode<T>> parent,
                std::shared_ptr<TreeNode<T>> node, bool isLeftChild)
        : tree(tree), parent(parent), node(node), isLeftChild(isLeftChild) {}

      void apply() override {
        if (node->isAlive()) {
          ERR("node is already alive");
        }

        if (!parent) {
          // Setting as root - parent is the tree itself
          if (tree->root) {
            ERR("root already exists");
          }
          tree->root = node;
          node->parent = tree;  // Root's parent is the Tree
        } else {
          // Setting as child
          if (isLeftChild) {
            if (parent->leftChild) {
              ERR("left child already exists");
            }
            parent->leftChild = node;
          } else {
            if (parent->rightChild) {
              ERR("right child already exists");
            }
            parent->rightChild = node;
          }
          node->parent = parent;
        }
        node->setAlive(true);
        if (node->element) node->element->setAlive(true); // TODO: ensure this is the only place setting element alive state
      }

      void undo() override {
        if (!node->isAlive()) {
          ERR("node is not alive");
        }

        if (!parent) {
          // Removing root
          if (tree->root != node) {
            ERR("node is not the root");
          }
          tree->root = nullptr;
        } else {
          // Removing child
          if (isLeftChild) {
            if (parent->leftChild != node) {
              ERR("node is not the left child");
            }
            parent->leftChild = nullptr;
          } else {
            if (parent->rightChild != node) {
              ERR("node is not the right child");
            }
            parent->rightChild = nullptr;
          }
          node->parent = nullptr;
        }
        node->setAlive(false);
        if (node->element) node->element->setAlive(false); // TODO: ensure this is the only place setting element alive state
      }
    };

    // RemoveNodeOp - removes a node (inverse of AddNodeOp)
    struct RemoveNodeOp : public AddNodeOp {
      RemoveNodeOp(std::shared_ptr<Tree<T>> tree, std::shared_ptr<TreeNode<T>> parent,
                   std::shared_ptr<TreeNode<T>> node, bool isLeftChild)
        : AddNodeOp(tree, parent, node, isLeftChild) {}

      void apply() override {
        AddNodeOp::undo();
      }

      void undo() override {
        AddNodeOp::apply();
      }
    };

    // clear all nodes
    void clear(Operation &op) {
      if (!root) return;

      std::queue<std::shared_ptr<TreeNode<T>>> q;
      removeNode(op, nullptr, root, false);
      q.push(root);
      while (!q.empty()) {
        std::shared_ptr<TreeNode<T>> node = q.front();
        q.pop();
        if (node->leftChild) {
          removeNode(op, node, node->leftChild, true);
          q.push(node->leftChild);
        }
        if (node->rightChild) {
          removeNode(op, node, node->rightChild, false);
          q.push(node->rightChild);
        }
      }
    }

  protected:
    Tree() : Display() {
      root = nullptr;
    }

    FRIEND_CREATE

  public:
    void draw() override {
      const float NODE_SPACING = 20.f;
      const float LEVEL_SPACING = 100.f;
      const int MARGIN = 10;

      if (!root || !root->isAlive()) {
        // Draw empty tree symbol (empty circle)
        const int SIZE = 50;
        resetCanvas(SIZE, SIZE);

        const float R = (float)SIZE * 0.4f;
        const float W = R * 0.3f;
        sf::CircleShape circle(R);
        circle.setOutlineThickness(-W);
        circle.setOutlineColor(sf::Color::Black);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOrigin({R, R});
        circle.setPosition({(float)SIZE * 0.5f, (float)SIZE * 0.5f});
        canvas.draw(circle);

        canvas.display();
        return;
      }

      // Calculate layout
      float treeWidth = root->calculateSubtreeWidth(NODE_SPACING);
      float treeHeight = calculateTreeHeight(root) * LEVEL_SPACING;

      // Layout the tree starting from center top
      root->layoutSubtree(treeWidth / 2.f, 0.f, NODE_SPACING, LEVEL_SPACING);

      // Find actual min and max X positions
      float minX = findMinX(root);
      float maxX = findMaxX(root);

      // Adjust if minimum X is negative (shift everything right)
      if (minX < 0) {
        adjustXOffsets(root, -minX);
        maxX += -minX;
        minX = 0.f;
      }

      // Calculate canvas size based on actual bounds
      float actualWidth = maxX - minX;
      int canvasWidth = (int)actualWidth + MARGIN * 2;
      int canvasHeight = (int)treeHeight + MARGIN * 2;

      resetCanvas(canvasWidth, canvasHeight);

      // Draw edges first, then nodes (no background - transparent)
      drawEdges(root, MARGIN, MARGIN);
      drawNodes(root, MARGIN, MARGIN);

      canvas.display();
    }

    // Add node to tree
    void addNode(Operation &op, std::shared_ptr<TreeNode<T>> parent,
                 std::shared_ptr<TreeNode<T>> node, bool isLeftChild) {
      op.comps.push_back(std::make_unique<AddNodeOp>(
        std::dynamic_pointer_cast<Tree<T>>(this->shared_from_this()), parent, node, isLeftChild
      ));
    }

    // Remove node from tree
    void removeNode(Operation &op, std::shared_ptr<TreeNode<T>> parent,
                    std::shared_ptr<TreeNode<T>> node, bool isLeftChild) {
      op.comps.push_back(std::make_unique<RemoveNodeOp>(
        std::dynamic_pointer_cast<Tree<T>>(this->shared_from_this()), parent, node, isLeftChild
      ));
    }

  private:
    // Calculate height of tree (number of levels)
    float calculateTreeHeight(std::shared_ptr<TreeNode<T>> node) {
      if (!node || !node->isAlive()) return 0.f;

      float leftHeight = calculateTreeHeight(node->leftChild);
      float rightHeight = calculateTreeHeight(node->rightChild);

      return 1.f + std::max(leftHeight, rightHeight);
    }

    // Find the minimum X offset in the tree
    float findMinX(std::shared_ptr<TreeNode<T>> node) {
      if (!node || !node->isAlive()) return 0.f;

      float minX = node->xOffset;

      if (node->leftChild && node->leftChild->isAlive()) {
        minX = std::min(minX, findMinX(node->leftChild));
      }
      if (node->rightChild && node->rightChild->isAlive()) {
        minX = std::min(minX, findMinX(node->rightChild));
      }

      return minX;
    }

    // Find the maximum X offset (right edge) in the tree
    float findMaxX(std::shared_ptr<TreeNode<T>> node) {
      if (!node || !node->isAlive()) return 0.f;

      float maxX = node->xOffset + node->getBBox().size.x;

      if (node->leftChild && node->leftChild->isAlive()) {
        maxX = std::max(maxX, findMaxX(node->leftChild));
      }
      if (node->rightChild && node->rightChild->isAlive()) {
        maxX = std::max(maxX, findMaxX(node->rightChild));
      }

      return maxX;
    }

    // Adjust all X offsets by a given amount
    void adjustXOffsets(std::shared_ptr<TreeNode<T>> node, float adjustment) {
      if (!node || !node->isAlive()) return;

      node->xOffset += adjustment;

      if (node->leftChild && node->leftChild->isAlive()) {
        adjustXOffsets(node->leftChild, adjustment);
      }
      if (node->rightChild && node->rightChild->isAlive()) {
        adjustXOffsets(node->rightChild, adjustment);
      }
    }

    // Draw edges between nodes
    void drawEdges(std::shared_ptr<TreeNode<T>> node, float offsetX, float offsetY) {
      if (!node || !node->isAlive()) return;

      auto nodeBBox = node->getBBox();
      float nodeX = offsetX + node->xOffset;
      float nodeY = offsetY + node->yOffset;
      float nodeCenterX = nodeX + nodeBBox.size.x / 2.f;
      float nodeCenterY = nodeY + nodeBBox.size.y / 2.f;

      // Draw edge to left child
      if (node->leftChild && node->leftChild->isAlive()) {
        auto childBBox = node->leftChild->getBBox();
        float childX = offsetX + node->leftChild->xOffset;
        float childY = offsetY + node->leftChild->yOffset;
        float childCenterX = childX + childBBox.size.x / 2.f;
        float childCenterY = childY + childBBox.size.y / 2.f;

        drawLine(nodeCenterX, nodeCenterY, childCenterX, childCenterY);
        drawEdges(node->leftChild, offsetX, offsetY);
      }

      // Draw edge to right child
      if (node->rightChild && node->rightChild->isAlive()) {
        auto childBBox = node->rightChild->getBBox();
        float childX = offsetX + node->rightChild->xOffset;
        float childY = offsetY + node->rightChild->yOffset;
        float childCenterX = childX + childBBox.size.x / 2.f;
        float childCenterY = childY + childBBox.size.y / 2.f;

        drawLine(nodeCenterX, nodeCenterY, childCenterX, childCenterY);
        drawEdges(node->rightChild, offsetX, offsetY);
      }
    }

    // Draw nodes recursively
    void drawNodes(std::shared_ptr<TreeNode<T>> node, float offsetX, float offsetY) {
      if (!node || !node->isAlive()) return;

      float nodeX = offsetX + node->xOffset;
      float nodeY = offsetY + node->yOffset;

      node->drawOn(canvas, nodeX, nodeY, shared_from_this());

      if (node->leftChild) {
        drawNodes(node->leftChild, offsetX, offsetY);
      }
      if (node->rightChild) {
        drawNodes(node->rightChild, offsetX, offsetY);
      }
    }

    // Helper to draw a line between two points
    void drawLine(float x1, float y1, float x2, float y2) {
      float dx = x2 - x1;
      float dy = y2 - y1;
      float length = std::sqrt(dx * dx + dy * dy);
      float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;

      sf::RectangleShape line({length, 2.f});
      line.setPosition({x1, y1});
      line.setRotation(sf::degrees(angle));
      line.setFillColor(sf::Color::Black);
      canvas.draw(line);
    }
  };
}
