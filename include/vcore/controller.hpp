#pragma once

#include "vcore/model.hpp"
#include "vcore/view.hpp"

// control the flow of the program, handle stepping forward and backward
namespace vcore {
  struct Controller {
    Model model;
    View view;
    int currOp = 0;
    Controller();
    
    void step(int amount);
    // void spin(); // wait until next operation needed (handles sfml events while waiting)
    // void stepForward();
    // void stepBackward();
    // void jump(int op);
  };

  extern Controller controller; // global controller object
}