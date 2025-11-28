#pragma once

#include "vobj/root_display.hpp"
#include "vobj/loc_info.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <type_traits>

namespace vobj {
  // represents individual components of operation display (such as highlighting element, arrow between operands, etc)
  // logic for applying and reversing operation goes here
  struct OpComp {
    virtual void apply() = 0;
    virtual void undo() = 0;
    virtual ~OpComp() = default;
    virtual void draw(std::shared_ptr<vobj::RootDisplay> root, sf::RenderTarget &t); // differs from display draw since uses global coordinates
  };

  // represents a single operation at a line number and offset
  struct Operation {
    size_t oid; // operation unique id

    LocInfo startLoc;
    LocInfo endLoc;

    std::string content; // text description of operation
    std::vector<std::unique_ptr<OpComp>> comps; // represent components of operation applied in order

    Operation();
    Operation(size_t oid, std::string content, LocInfo startLoc, LocInfo endLoc);
    Operation(size_t oid, std::string content, LocInfo loc);

    void draw(std::shared_ptr<vobj::RootDisplay> root, sf::RenderTarget &t);
    void apply();
    void undo();
  };
}
