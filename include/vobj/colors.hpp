#pragma once

#include <SFML/Graphics.hpp>

namespace vobj {

struct ColorScheme {
  sf::Color background;
  sf::Color highlightOutline;
  sf::Color nameText;
};

inline ColorScheme defaultScheme() {
  return {sf::Color(200, 200, 255), sf::Color::Red, sf::Color::Black};
}

inline ColorScheme colorblindScheme() {
  return {sf::Color(220, 220, 255), sf::Color(0, 114, 178), sf::Color(0, 0, 0)};
}

inline bool &colorblindMode() {
  static bool mode = false;
  return mode;
}

inline const ColorScheme &currentScheme() {
  static ColorScheme normal = defaultScheme();
  static ColorScheme cb = colorblindScheme();
  return colorblindMode() ? cb : normal;
}

} // namespace vobj
