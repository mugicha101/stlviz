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
    size_t oid; // operation unique id

    uint64_t startLine;
    uint64_t startOffset;
    uint64_t endLine;
    uint64_t endOffset;

    std::string content; // text description of operation
    std::vector<std::unique_ptr<OpComp>> comps; // represent components of operation applied in order

    Operation();
    Operation(size_t oid, uint64_t line, uint64_t offset, std::string content = "");
    Operation(size_t oid, uint64_t startLine, uint64_t startOffset, uint64_t endLine, uint64_t endOffset, std::string content = "");

    void draw() override;
    void apply();
    void undo();
  };
}
