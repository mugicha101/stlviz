#include <vcore/controller.hpp>
#include <iostream>

namespace vcore {
  Controller::Controller() : view(800u, 600u) {
    std::cout << "CONTROLLER INIT" << std::endl;
  }

  void Controller::step(int amount) {
    std::cout << "STEP " << amount << std::endl;
  }

  Controller controller;
}