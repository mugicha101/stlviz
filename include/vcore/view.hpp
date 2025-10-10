#pragma once

// render screen using SFML
#include <SFML/Graphics.hpp>
#include "vobj/all.hpp"
#include <memory>

namespace vcore {
  struct View {
    sf::RenderWindow window;

    View(uint32_t width = 0, uint32_t height = 0);

    void update();
  };
}