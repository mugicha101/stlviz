#pragma once

#include "vobj/display.hpp"
#include "vobj/backing_type.hpp"
#include <map>
#include <iostream>
#include <algorithm>

namespace vobj {
  template<typename T>
  struct List : public Display {
    std::map<size_t, std::shared_ptr<T>> elements; // index -> element

    // add element at index
    // index must be empty
    // updates alive to be true
    struct AddOp : public OpComp {
      std::shared_ptr<List<T>> list;
      size_t index;
      std::shared_ptr<T> element;

      AddOp(std::shared_ptr<List<T>> list, size_t index, std::shared_ptr<T> element) : list(list), index(index), element(element) {

      }

      void apply() override {
        auto &elements = list->elements;
        if (element->alive) {
          ERR("element is already alive");
        }
        if (elements.find(index) != elements.end()) {
          ERR("element already exists at index " + std::to_string(index));
        }

        elements[index] = element;
        element->parent = list;
        element->alive = true;
      }

      void undo() override {
        auto &elements = list->elements;
        if (!element->alive) {
          ERR("element is not alive");
        }
        if (elements.find(index) == elements.end() || elements[index] != element) {
          ERR("element doesn't exist at index " + std::to_string(index));
        }

        elements.erase(index);
        element->parent = nullptr;
        element->alive = false;
      }
    };

    // remove element from index
    // just inverse of AddOp
    struct RemoveOp : public AddOp {
      RemoveOp(std::shared_ptr<List<T>> list, size_t index, std::shared_ptr<T> element) : AddOp(list, index, element) {

      }

      void apply() override {
        AddOp::undo();
      }

      void undo() override {
        AddOp::apply();
      }
    };
    
  protected:

    List() : Display() {};

    FRIEND_CREATE

  public:

    void draw() override {
      const int cellPadding = 5;
      const int cellBorder = 2;

      if (elements.empty()) {
        // draw empty set symbol
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

        sf::RectangleShape line({(float)SIZE * 1.2f, W});
        line.setOrigin({line.getSize().x * 0.5f, line.getSize().y * 0.5f});
        line.setPosition({(float)SIZE * 0.5f, (float)SIZE * 0.5f});
        line.setRotation(sf::degrees(-45));
        line.setFillColor(sf::Color::Black);
        canvas.draw(line);

        canvas.display();
        return;
      }

      int height = 0;
      int width = cellBorder;
      for (auto &[i, e] : elements) {
        auto bbox = e->getBBox();
        width += bbox.size.x + cellPadding * 2 + cellBorder;
        height = std::max(height, bbox.size.y);
      }

      int margin = (cellBorder + cellPadding) * 2;
      height += margin;

      resetCanvas(width, height);

      int x = cellBorder;
      for (auto &[i, e] : elements) {
        auto bbox = e->getBBox();
        e->drawOn(canvas, (float)(x + cellPadding), (float)(height - bbox.size.y) * 0.5f, shared_from_this());
        sf::RectangleShape rect({(float)(bbox.size.x + cellPadding * 2), (float)(height - cellBorder * 2)});
        rect.setPosition({(float)x, (float)cellBorder});
        rect.setOutlineThickness(cellBorder);
        rect.setOutlineColor(sf::Color::Black);
        rect.setFillColor(sf::Color::Transparent);
        canvas.draw(rect);

        x += bbox.size.x + cellPadding * 2 + cellBorder;
      }
      canvas.display();
    };

    // add element to index
    void add(Operation &op, size_t index, std::shared_ptr<T> element) {
      op.comps.push_back(std::make_unique<AddOp>(
        std::dynamic_pointer_cast<List<T>>(this->shared_from_this()), index, element
      ));
    }

    // remove element from index
    void remove(Operation &op, size_t index) {
      op.comps.push_back(std::make_unique<RemoveOp>(
        std::dynamic_pointer_cast<List<T>>(this->shared_from_this()), index, elements.at(index)
      ));
    }
  };
}
