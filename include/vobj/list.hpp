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
          throw std::runtime_error("element is already alive");
        }
        if (elements.find(index) != elements.end()) {
          throw std::runtime_error("element already exists at index " + std::to_string(index));
        }

        elements[index] = element;
        element->parent = list;
        element->alive = true;
      }

      void undo() override {
        auto &elements = list->elements;
        if (!element->alive) {
          throw std::runtime_error("element is not alive");
        }
        if (elements.find(index) == elements.end() || elements[index] != element) {
          throw std::runtime_error("element doesn't exist at index " + std::to_string(index));
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
      int height = 0;
      int width = 0;
      for (auto &[i, e] : elements) {
        bbox = e->getBBox();
        width += bbox.size.x;
        height = std::max(height, bbox.size.y);
      }

      const int cellBorder = 5;
      height += cellBorder * 2;

      resetCanvas(width, height);
      int x = 0;
      for (auto &[i, e] : elements) {
        bbox = e->getBBox();
        e->drawOn(canvas, (float)x, (float)(height - bbox.size.y) * 0.5f);
        x += bbox.size.x;
      }
      sf::RectangleShape rect({50.f, 150.f});
      rect.setFillColor(sf::Color(0, 0, 255));
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
