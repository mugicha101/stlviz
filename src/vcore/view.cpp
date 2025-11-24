#include <vcore/all.hpp>
#include <iostream>

namespace vcore {
  View::View(uint32_t width, uint32_t height) {
    window.create(sf::VideoMode({width, height}), "STLVIZ");
    window.setFramerateLimit(60);
  }

  void View::update(std::shared_ptr<vobj::Display> root, std::deque<vobj::Operation> &ops, size_t currOp) {
    // draw data window
    ++vobj::Display::globalDrawTick;
    window.clear(sf::Color(200, 200, 255));
    root->drawOn(window, 0.f, 0.f);

    // draw last operation to window
    if (currOp > 0) ops[currOp-1].draw(window);

    // draw operations list
    // TODO

    window.display();
  }
}