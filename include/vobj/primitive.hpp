#pragma once

#include "vobj/display.hpp"
#include <iostream>
#include <string>

namespace vobj {
  template<typename T>
  struct AssignOp;

  // backing class of primitive values
  template<typename T>
  struct Primitive : public Display {
    T value{}; // represents value at current operation
    T latest{}; // represents actual value in main thread

protected:

    Primitive() : Display() {}
    Primitive(T v) : value(v), latest(v) {}

    FRIEND_CREATE

public:

    ~Primitive() {}

    // override update since no vstd::base
    // assignment of latest should be considered in parent by calling update(op, realLatest)
    virtual bool update(Operation &op) override {
      throw std::runtime_error("Primitive<T>::update(op) should not be called directly, Primitive<T>::update(op, val) instead!");
    }

    bool update(Operation &op, T realLatest) {
      if (localUpdateTick == globalUpdateTick) return updated;
      localUpdateTick = globalUpdateTick;
      updated = latest != realLatest;
      if (updated) {
        op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(std::dynamic_pointer_cast<Primitive<T>>(this->shared_from_this()), latest, realLatest));
        latest = realLatest;
      }
      return updated;
    }

    void draw() override {
      sf::Font &font = Display::getFont();
      sf::Text text(font);
      text.setString(std::to_string(value));
      text.setCharacterSize(FONT_SIZE);
      float charWidth = font.getGlyph('0', FONT_SIZE, false).advance;
      uint32_t width = (uint32_t)std::ceil(charWidth * (float)text.getString().getSize());
      uint32_t height = font.getLineSpacing(FONT_SIZE);
      resetCanvas(width, height);
      text.setFillColor(sf::Color::Black);
      canvas.draw(text);
      canvas.display();
    };
  };
}
