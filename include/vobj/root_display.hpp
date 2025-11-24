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

    sf::Vector2i size{800, 600};

    // draw all displays recursively
    // each call to this represents a draw tick (increments globalDrawTick)
    void draw() override;

    // updates all displays recursively
    // each call to this represents an update tick (increments globalUpdateTick)
    bool update(Operation &op) override;
  };
}
