#pragma once

// represent model objects
#include "vobj/all.hpp"

namespace vcore {
  struct Model {
    std::shared_ptr<vobj::RootDisplay> root = vobj::create<vobj::RootDisplay>();
    std::deque<vobj::Operation> ops;
    Model();
  };
}