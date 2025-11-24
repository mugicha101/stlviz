#include "vobj/operation.hpp"
#include <iostream>

namespace vobj {
  void OpComp::draw(sf::RenderTarget &t) {}

  Operation::Operation() : Operation(0, 0, 0, "") {
    
  }

  Operation::Operation(size_t oid, uint64_t line, uint64_t offset, std::string content) : Operation(oid, line, offset, line, offset, content) {
    
  }

  Operation::Operation(size_t oid, uint64_t startLine, uint64_t startOffset, uint64_t endLine, uint64_t endOffset, std::string content) : oid(oid), startLine(startLine), startOffset(startOffset), endLine(endLine), endOffset(endOffset), content(content) {
    
  }

  void Operation::draw(sf::RenderTarget &t) {
    for (auto &comp : comps) {
      comp->draw(t);
    }
  }

  void Operation::apply() {
    std::cout << "APPLY " << content << " " << comps.size() << std::endl;
    for (auto &comp : comps) {
      comp->apply();
    }
  }

  void Operation::undo() {
    std::cout << "UNDO " << content << " " << comps.size() << std::endl;
    for (auto it = comps.rbegin(); it != comps.rend(); ++it) {
      (*it)->undo();
    }
  }
}
