#pragma once

#include "vcore/model.hpp"
#include "vcore/view.hpp"

// control the flow of the program, handle stepping forward and backward
namespace vcore {
  struct Controller {
    static Controller &globalController();
    Model model;
    View view;
    size_t currOp = 0; // current state is right before this operation
    size_t targetOp = 0; // target state is right before this operation
    bool mousePressed = false;
    sf::Vector2f mousePos = {0.f, 0.f};
    bool mainDone = false;
    std::shared_ptr<vobj::Display> selectedDisplay = nullptr;
    Controller();
    ~Controller();
    
    void step(long long amount);
    void spin(); // wait until next operation needed (handles sfml events while waiting)
    void update(std::source_location sloc); // check for changed values
    void jump(size_t op);
    void mouseDown();
    void mouseUp();
    void mouseMove(float x, float y);
    void mouseScroll(float delta);
  };
}