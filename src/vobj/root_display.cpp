#include "vobj/root_display.hpp"
#include <iostream>

namespace vobj {
  RootDisplay::RootDisplay() {
    alive = true; // always alive
  }

  void RootDisplay::draw() {
    resetCanvas(size.x, size.y);
    int y = 0;
    for (std::shared_ptr<Display> d : Display::displays) {
      // Skip displays that have a parent, are not alive, or are this RootDisplay itself
      if (d->parent || !d->alive || d.get() == this) continue;
      
      bbox = d->getBBox();

      // draw name
      sf::Font &font = Display::getFont();
      sf::Text text(font);
      text.setString(d->name + ": ");
      text.setCharacterSize(FONT_SIZE);
      float charWidth = font.getGlyph('0', FONT_SIZE, false).advance;
      float nameWidth = charWidth * (float)text.getString().getSize();
      float nameHeight = (float)font.getLineSpacing(FONT_SIZE);
      text.setFillColor(sf::Color::Black);
      text.setPosition({0.f, (float)y + (float)bbox.size.y * 0.5f});
      text.setOrigin({0.f, nameHeight * 0.5f});
      canvas.draw(text);
      canvas.display();

      // draw display
      d->drawOn(canvas, nameWidth, (float)y, shared_from_this());
      y += bbox.size.y;
    }
    canvas.display();
  }

  bool RootDisplay::update(Operation &op) {
    for (std::shared_ptr<Display> d : Display::displays) {
      // Skip displays that have a parent, are not alive, or are this RootDisplay itself
      if (d->parent || !d->alive || d.get() == this) continue;

      std::cout << "UPDATE TOP LEVEL " << d->uid << std::endl;
      d->update(op);
    }
    return false;
  }
}
