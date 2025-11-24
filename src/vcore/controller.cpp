#include <vcore/controller.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace vcore {
  Controller::Controller() : view(800u, 600u) {}
  Controller::~Controller() {
  }

  void Controller::spin() {
    while (view.window.isOpen() && targetOp <= model.ops.size()) {
      while (const std::optional event = view.window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
          view.window.close();
          break;
        } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
          using Key = sf::Keyboard::Scan;
          switch (keyPressed->scancode) {
            case Key::Escape:
              view.window.close();
              break;
            case Key::Right:
              step(1);
              break;
            case Key::Left:
              step(-1);
              break;
            default:
              break;
          }
        }
      }

      // step currOp towards targetOp
      while (currOp != targetOp && !(currOp == model.ops.size() && targetOp > model.ops.size())) {
        if (currOp < targetOp) {
          model.ops[currOp++].apply();
        } else {
          model.ops[--currOp].undo();
        }
      }

      view.update(model.root, model.ops, currOp);
    }

    if (!view.window.isOpen()) {
      std::cout << "WINDOW CLOSED" << std::endl;
      //using _Exit(0) since it calls static destructorswhich run in wrong order, leading to exit error
      _Exit(0);
    }
  }

  void Controller::update(std::source_location sloc) {
    model.update(sloc);
  }

  void Controller::step(long long amount) {
    std::cout << "STEP " << amount << std::endl;
    if (amount < 0 && -amount > targetOp) {
      targetOp = 0;
    } else {
      targetOp += amount;
    }
  }

  void Controller::jump(size_t op) {
    targetOp = std::max((size_t)0, op);
  }

  Controller controller;
}