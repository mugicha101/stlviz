#pragma once

#include "vobj/display.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <type_traits>

namespace vobj {
  // represents individual components of operation display (such as highlighting element, arrow between operands, etc)
  // logic for applying and reversing operation goes here
  struct OpComp : public Display {
    virtual void apply() = 0;
    virtual void undo() = 0;
  };

  // represents a single operation at a line number and offset
  struct Operation : public Display {
    uint64_t line;
    uint64_t offset;
    std::string content;
    std::vector<std::unique_ptr<OpComp>> comps; // represent components of operation applied in order
    Operation(uint64_t line = 0, uint64_t offset = 0, std::string content = "");
    void draw(sf::RenderTarget &c, sf::Transform t) override;
    void apply();
    void undo();
  };
}
