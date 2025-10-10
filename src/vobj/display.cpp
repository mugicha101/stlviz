#include "vobj/display.hpp"
#include "vstd/base.hpp"

namespace vobj {
  std::deque<std::shared_ptr<Display>> Display::displays;

  Display::Display() {}

  bool Display::update(Operation &op) {
    return o && o->_vstd_update_values(op);
  }
}
