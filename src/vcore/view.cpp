#include <vcore/view.hpp>
#include <iostream>

namespace vcore {
  View::View(uint32_t width, uint32_t height) {
    window.create(sf::VideoMode({width, height}), "STLVIZ");
    window.setFramerateLimit(60);
  }

  void View::update(std::shared_ptr<vobj::Display> root) {
    ++vobj::Display::globalDrawTick;
    window.clear(sf::Color(200, 200, 255));
    root->drawOn(window, 0, 0);
    window.display();
  }
}