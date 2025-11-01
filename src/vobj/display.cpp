#include "vobj/display.hpp"
#include "vstd/base.hpp"
#include <algorithm>
#include <iostream>

namespace vobj {
  std::deque<std::shared_ptr<Display>> Display::displays;
  uint64_t Display::globalTick;

  Display::Display() : localTick(globalTick), canvas({32u, 32u}) {}

  sf::Font &Display::getFont() {
    static sf::Font font;
    static bool fontLoaded = false;
    if (!fontLoaded) {
      fontLoaded = true;
      const std::string path = std::string() + STLVIZ_ASSETS_DIR + "/Cascadia_Code/CascadiaCode-VariableFont_wght.ttf";
      if (!font.openFromFile(path)) {
        std::cout << "FAILED TO LOAD FONT FROM PATH " << path << std::endl;
      }
    }
    return font;
  }

  bool Display::update(Operation &op) {
    return o && o->_vstd_update_values(op);
  }

  void Display::drawOn(sf::RenderTarget &t, int x, int y) {
    // TODO: figure out why setTextureRect behaves weird
    sf::Sprite sprite(getTexture());
    sprite.setPosition({(float)x, (float)y});
    t.draw(sprite);
  }

  const sf::Texture &Display::getTexture() {
    if (localTick != globalTick) {
      localTick = globalTick;
      draw();
    }

    return canvas.getTexture();
  }

  sf::IntRect Display::getBBox() {
    if (localTick != globalTick) {
      localTick = globalTick;
      draw();
    }

    return bbox;
  }

  void Display::drawAll(sf::RenderTarget &c) {
    ++globalTick;
    for (std::shared_ptr<Display> d : displays) {
      if (d->parent || !d->alive) continue;

      d->drawOn(c, 0, 0);
    }
  }

  void Display::resetCanvas(uint32_t width, uint32_t height) {
    assert(width <= (1u << 31));
    assert(height <= (1u << 31));
    if (canvas.getSize().x < width || canvas.getSize().y < height) {
      uint32_t allocWidth = std::max(canvas.getSize().x, (uint32_t)32u);
      uint32_t allocHeight = std::max(canvas.getSize().y, (uint32_t)32u);
      while (allocWidth < width) allocWidth <<= 1;
      while (allocHeight < height) allocHeight <<= 1;

      if (!canvas.resize({allocWidth, allocHeight})) {
        std::cout << "Display " << uid << ": canvas resize failed" << std::endl;
      }
    }
    canvas.clear(sf::Color::Transparent);
    bbox = sf::IntRect({0,0},{(int)width,(int)height});
  }

  void Display::draw() {
    resetCanvas(128, 128);
    sf::RectangleShape tile(sf::Vector2f(32.f, 32.f));
    const sf::Color magenta(255, 0, 255);
    const sf::Color black(0, 0, 0);
    bool f = false;
    for (uint32_t x = 0; x < 128; x += 32) {
      bool of = f;
      for (uint32_t y = 0; y < 128; y += 32) {
        tile.setPosition({(float)x, (float)y});
        tile.setFillColor(f ? magenta : black);
        f = !f;
        canvas.draw(tile);
      }
      f = !of;
    }
    canvas.display();
  }
}
