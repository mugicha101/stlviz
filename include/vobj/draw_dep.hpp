#pragma once

#include <SFML/Graphics.hpp>

// stores position a display is drawn at and the parent its drawn on
namespace vobj {
  struct Display;
  struct DrawDep {
    float x;
    float y;
    std::shared_ptr<vobj::Display> parent;
  };
}