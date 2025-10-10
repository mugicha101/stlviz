#include <vcore/view.hpp>
#include <iostream>

namespace vcore {
  View::View(uint32_t width, uint32_t height) {
    window.create(sf::VideoMode({width, height}), "STLVIZ");
    window.setFramerateLimit(60);
  }

  void View::update() {
    window.clear(sf::Color::Red);
    window.display();
  }
}