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

      resetCanvas(width, height);
      int x = 0;
      for (auto &[i, e] : elements) {
        bbox = e->getBBox();
        std::cout << i << " drawn at " << x << std::endl;
        e->drawOn(canvas, x, (height - bbox.size.y) >> 1);
        x += bbox.size.x;
      }
      sf::RectangleShape rect({50.f, 150.f});
      rect.setFillColor(sf::Color(0, 0, 255));
      canvas.display();
    };
  };
}
