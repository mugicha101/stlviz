#pragma once

#include "vobj/display.hpp"
#include <iostream>

namespace vobj {
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
    virtual bool update(Operation &op) override {
      if (value != latest) {
        // TODO: push assignment op
        return true;
      }
      return false;
    }

    void draw(sf::RenderTarget &c, sf::Transform t) override {
      // UNKNOWN PRIMITIVE (might be user struct)
    };
  };
}
