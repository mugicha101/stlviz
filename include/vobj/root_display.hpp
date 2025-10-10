#pragma once

#include "vobj/display.hpp"

namespace vobj {
  // singleton root display, holds all other displays
  struct RootDisplay : public Display {
  protected:

    RootDisplay();

    FRIEND_CREATE

  public:

    void draw(sf::RenderTarget &c, sf::Transform t) override;
  };
}
