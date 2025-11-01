#pragma once

#include "vcore/model.hpp"
#include "vcore/view.hpp"

// control the flow of the program, handle stepping forward and backward
namespace vcore {
  struct Controller {
    Model model;
    View view;
    size_t currOp = 0; // current state is right before this operation
    size_t targetOp = 0; // target state is right before this operation
    Controller();
    
    void step(long long amount);
    void spin(); // wait until next operation needed (handles sfml events while waiting)
    void jump(size_t op);
  };

  extern Controller controller;
}