#pragma once

#include "vobj/colors.hpp"
#include "vobj/display.hpp"
#include "vobj/operation.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include <utility>

namespace vobj {
  template<typename T>
  struct AssignOp;

  template<typename T, typename Enable = void>
  struct ToString {
    static std::string apply(const T &val) {
      return std::to_string(val);
    }
  };

  template<>
  struct ToString<std::string> {
    static std::string apply(const std::string &val) {
      return val;
    }
  };

  template<typename T>
  struct ToString<T, std::enable_if_t<std::is_floating_point<T>::value>> {
    static std::string apply(const T &val) {
      std::ostringstream ss;
      ss << std::setprecision(17) << std::defaultfloat << val;
      return ss.str();
    }
  };

  template<typename A, typename B>
  struct ToString<std::pair<A,B>> {
    static std::string apply(const std::pair<A,B> &val) {
      return ToString<A>::apply(val.first) + "," + ToString<B>::apply(val.second);
    }
  };

  // parent class of primitive to allow for runtime detection for primitives
  struct PrimitiveBase : public Display {
    // override update since no vstd::base
    // assignment of latest should be considered in parent by calling update(op, realLatest)
    bool update(Operation &op) override {
      ERR("Primitive<T>::update(op) should not be called directly, Primitive<T>::update(op, val) instead!");
    }
  };

  // backing class of primitive values
  template<typename T>
  struct Primitive : public PrimitiveBase {
    T value{}; // represents value at current operation
    T latest{}; // represents actual value in main thread

protected:

    Primitive() : PrimitiveBase() {}
    Primitive(T v) : value(v), latest(v) {}

    FRIEND_CREATE

public:

    bool update(Operation &op) override {
      return PrimitiveBase::update(op);
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
      text.setString(ToString<T>::apply(value));
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
