#pragma once

// represent model objects
#include "vobj/all.hpp"
#include <source_location>

namespace vcore {
  struct Model {
    // std::shared_ptr<vobj::RootDisplay> root = vobj::create<vobj::RootDisplay>();
    std::deque<vobj::Operation> ops;
    Model();

    // add op
    vobj::Operation &addOp(std::source_location sloc, std::string content);

    // update all vobjs to check for changed values
    void update();
  };
}