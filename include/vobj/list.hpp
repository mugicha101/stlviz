#pragma once

#include "vobj/display.hpp"
#include "vobj/backing_type.hpp"
#include <map>
#include <iostream>

namespace vobj {
  template<typename T>
  struct List : public Display {
    std::map<size_t, std::shared_ptr<T>> elements; // index -> element
    
  protected:

    List() : Display() {};

    FRIEND_CREATE

  public:

  void draw(sf::RenderTarget &c, sf::Transform t) override {
      // TODO
    };
  };
}
