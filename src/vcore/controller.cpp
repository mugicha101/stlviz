#include <vcore/controller.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>

namespace vcore {
  Controller::Controller() : view(sf::VideoMode::getDesktopMode().size / 2u) {
    view.window.setFramerateLimit(60);
  }
  Controller::~Controller() {}

  Controller &Controller::globalController() {
    static Controller controller;
    return controller;
  }

  void Controller::spin() {
    auto updateCurrOp = [this]() {
      // step currOp towards targetOp
      while (currOp != targetOp && !(currOp == model.ops.size() && targetOp > model.ops.size())) {
        if (currOp < targetOp) {
          model.ops[currOp++].apply();
        } else {
          model.ops[--currOp].undo();
        }
      }
    };
    updateCurrOp();
    bool indefiniteWait = mainDone && vobj::Display::getNumAlive() == 0;
    while (view.window.isOpen() && (indefiniteWait || targetOp <= model.ops.size())) {
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
        } else if (const auto *resize = event->getIf<sf::Event::Resized>()) {
            // update the view to the new size of the window
            sf::FloatRect visibleArea(sf::Vector2f{0.f, 0.f}, sf::Vector2f{(float)resize->size.x, (float)resize->size.y});
            view.window.setView(sf::View(visibleArea));
        } else if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
          mouseMove((float)mouseMoved->position.x, (float)mouseMoved->position.y);
        } else if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
          if (mouseButtonPressed->button == sf::Mouse::Button::Left) mouseDown();
        } else if (const auto *mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
          if (mouseButtonReleased->button == sf::Mouse::Button::Left) mouseUp();
        } else if (const auto *mouseScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
          static float delta = 0.f;
          delta += mouseScrolled->delta;
          int amt = (delta >= 0.f ? 1 : -1) * (int)std::abs(delta);
          if (amt != 0) {
            step(-amt);
            delta -= amt;
          }
        }
      }

      updateCurrOp();
      indefiniteWait = mainDone && vobj::Display::getNumAlive() == 0;
      if (indefiniteWait) targetOp = std::min(targetOp, model.ops.size());
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
    std::cout << "JUMP " << op << std::endl;
    targetOp = std::max((size_t)0, op);
  }

  void Controller::mouseDown() {
    if (mousePressed) return;
    mousePressed = true;

    int selectedOp = view.opListHover(mousePos.x, mousePos.y);
    if (selectedOp >= 0) {
      jump((size_t)selectedOp);
    }
  }

  void Controller::mouseUp() {
    if (!mousePressed) return;
    mousePressed = false;
  }

  void Controller::mouseMove(float x, float y) {
    mousePos = {x, y};
  }
}