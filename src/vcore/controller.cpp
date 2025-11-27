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
            // Tab navigation with number keys
            case Key::Num1:
              if (view.tabs.size() >= 1) view.activeTabIndex = 0;
              break;
            case Key::Num2:
              if (view.tabs.size() >= 2) view.activeTabIndex = 1;
              break;
            case Key::Num3:
              if (view.tabs.size() >= 3) view.activeTabIndex = 2;
              break;
            case Key::Num4:
              if (view.tabs.size() >= 4) view.activeTabIndex = 3;
              break;
            case Key::Num5:
              if (view.tabs.size() >= 5) view.activeTabIndex = 4;
              break;
            case Key::Num6:
              if (view.tabs.size() >= 6) view.activeTabIndex = 5;
              break;
            // Tab/Shift+Tab for next/previous tab
            case Key::Tab:
              if (keyPressed->shift) {
                // Previous tab
                if (view.activeTabIndex > 0) {
                  view.activeTabIndex--;
                } else {
                  view.activeTabIndex = view.tabs.size() - 1;
                }
              } else {
                // Next tab
                view.activeTabIndex = (view.activeTabIndex + 1) % view.tabs.size();
              }
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
          mouseScroll(mouseScrolled->delta);
        }
      }

      updateCurrOp();
      indefiniteWait = mainDone && vobj::Display::getNumAlive() == 0;
      if (indefiniteWait) targetOp = std::min(targetOp, model.ops.size());
      if (selectedDisplay && (!selectedDisplay->alive || selectedDisplay->parent)) selectedDisplay = nullptr;
      view.update(*this);
    }

    if (!view.window.isOpen()) {
      std::cout << "WINDOW CLOSED" << std::endl;
      //using _Exit(0) since it calls static destructorswhich run in wrong order, leading to exit error
      _Exit(0);
    }
  }

  bool Controller::update(std::source_location sloc) {
    return model.update(sloc);
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

    // Check if clicking on a tab
    int selectedTab = view.tabBarHover(mousePos.x, mousePos.y);
    if (selectedTab >= 0) {
      view.activeTabIndex = (size_t)selectedTab;
      return;
    }

    int selectedOp = view.opListHover(mousePos.x, mousePos.y);
    if (selectedOp >= 0) {
      jump((size_t)selectedOp);
    } else {
      // Adjust mouse position for tab bar offset
      sf::Vector2f adjustedMousePos = {mousePos.x, mousePos.y - (float)view.tabBarHeight};
      selectedDisplay = model.root->at(model.root->screen2world(adjustedMousePos));
      if (selectedDisplay) selectedDisplay->priority = vobj::Display::globalDrawTick;
    }
  }

  void Controller::mouseUp() {
    if (!mousePressed) return;
    mousePressed = false;
  }

  void Controller::mouseMove(float x, float y) {
    if (mousePressed) {
      if (selectedDisplay) {
        selectedDisplay->pos.x += (x - mousePos.x) * model.root->camZoom;
        selectedDisplay->pos.y += (y - mousePos.y) * model.root->camZoom;
      } else {
        // Adjust for tab bar offset
        sf::Vector2f oldAdjustedPos = {mousePos.x, mousePos.y - (float)view.tabBarHeight};
        sf::Vector2f newAdjustedPos = {x, y - (float)view.tabBarHeight};
        sf::Vector2f oldMouseWorldPos = model.root->screen2world(oldAdjustedPos);
        sf::Vector2f newMouseWorldPos = model.root->screen2world(newAdjustedPos);
        model.root->camPosition -= newMouseWorldPos - oldMouseWorldPos;
      }
    }
    mousePos = {x, y};
  }

  void Controller::mouseScroll(float delta) {
    if (mousePos.x >= view.opListX) {
      static float partialStep = 0.f;
      partialStep += delta;
      int amt = (partialStep >= 0.f ? 1 : -1) * (int)std::abs(partialStep);
      if (amt != 0) {
        step(-amt);
        partialStep -= amt;
      }
    } else {
      sf::Vector2f mouseWorldPos = model.root->screen2world(mousePos);
      float zoomFactor = pow(1.1f, -delta);
      model.root->camZoom *= zoomFactor;
      sf::Vector2f newMouseWorldPos = model.root->screen2world(mousePos);
      model.root->camPosition += mouseWorldPos - newMouseWorldPos;
    }
  }
}