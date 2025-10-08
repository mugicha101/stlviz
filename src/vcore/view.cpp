#include <vcore/view.hpp>
#include <iostream>

namespace vcore {
  View::View(uint32_t width, uint32_t height) {
    std::cout << "CREATE WINDOW" << std::endl;
    window.create(sf::VideoMode({width, height}), "STLVIZ");
    std::cout << "WINDOW OPEN" << std::endl;
    window.setFramerateLimit(60);
  }

  View::~View() {
    window.close();
  }

  void View::update() {
    window.clear(sf::Color::Red);
    window.display();
  }
}