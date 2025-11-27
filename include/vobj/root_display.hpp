#pragma once

#include "vobj/display.hpp"
#include <functional>

namespace vobj {
  // singleton root display, holds all displays not within another display
  struct RootDisplay : public Display {
  protected:

    RootDisplay();

    FRIEND_CREATE

    // gets alive top level displays in ascending priority order (tie break by uid)
    std::vector<std::shared_ptr<Display>> topLevelDisplays() const;
    std::vector<std::shared_ptr<Display>> topLevelDisplays(std::function<bool(std::shared_ptr<Display>)> filter) const;

  public:

    sf::Vector2i size{800, 600}; // canvas size (should match screen size)
    sf::Vector2f camPosition{0.f, 0.f}; // position of camera (world coordinates at top-left of screen)
    float camZoom = 1.f; // zoom level of camera
    std::function<bool(std::shared_ptr<Display>)> displayFilter = nullptr; // optional filter for which displays to show

    // draw all displays recursively
    // each call to this represents a draw tick (increments globalDrawTick)
    void draw() override;

    // updates all displays recursively
    // each call to this represents an update tick (increments globalUpdateTick)
    bool update(Operation &op) override;

    // gets top level display at position (x, y) in world coordinates
    std::shared_ptr<Display> at(sf::Vector2f worldPos) const;

    // transform between coordinate spaces
    sf::Vector2f screen2world(sf::Vector2f screenPos) const;
    sf::Vector2f world2screen(sf::Vector2f worldPos) const;
  };
}
