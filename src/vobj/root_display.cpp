#pragma once

#include "vobj/root_display.hpp"
#include <iostream>

namespace vobj {
  RootDisplay::RootDisplay() {
    alive = true; // always alive
  }

  void RootDisplay::draw() {
    resetCanvas(800u, 600u);
    int y = 0;
    for (std::shared_ptr<Display> d : Display::displays) {
      if (d->parent || !d->alive || d->uid == uid) continue;

      bbox = d->getBBox();
      d->drawOn(canvas, 0, 0);
      y += bbox.size.y;
    }
    canvas.display();
  }

  bool RootDisplay::update(Operation &op) {
    for (std::shared_ptr<Display> d : Display::displays) {
      if (d->parent || !d->alive || d->uid == uid) continue;
      
      std::cout << "UPDATE TOP LEVEL " << d->uid << std::endl;
      d->update(op);
    }
    return false;
  }
}
