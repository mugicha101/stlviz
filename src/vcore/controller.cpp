#include <vcore/controller.hpp>
#include <iostream>

namespace vcore {
  Controller::Controller() : view(800u, 600u) {}

  void Controller::spin() {
    while (view.window.isOpen()) {
      while (const std::optional event = view.window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
          view.window.close();
          break;
        }
      }

      view.update();
    }
  }

  void Controller::step(int amount) {
    std::cout << "STEP " << amount << std::endl;
  }

  Controller controller;
}