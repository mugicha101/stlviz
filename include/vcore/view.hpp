#pragma once

// render screen using SFML
#include <SFML/Graphics.hpp>
#include "vobj/all.hpp"
#include <memory>

namespace vcore {
  struct Model;
  struct Controller;

  struct View {
    sf::RenderWindow window;
    uint32_t opListX = 0u; // x position of operation list
    float opListYOff = 0.f; // vertical offset of operation list scrolling

    View(uint32_t width = 0, uint32_t height = 0);
    View(sf::Vector2u size) : View(size.x, size.y) {}

    void update(vcore::Controller &controller);
    int opListHover(float mouseX, float mouseY) const;
  };
}