#pragma once

#include "vobj/display.hpp"

namespace vobj {
  // singleton root display, holds all displays not within another display
  // TODO: actually use this rather than drawing all separately
  struct RootDisplay : public Display {
  protected:

    RootDisplay();

    FRIEND_CREATE

  public:
  };
}
