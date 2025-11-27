#include "vobj/operation.hpp"
#include <iostream>

namespace vobj {
  void OpComp::draw(std::shared_ptr<vobj::RootDisplay> root, sf::RenderTarget &t) {}

  Operation::Operation() : Operation(0, 0, 0, "") {
    
  }

  Operation::Operation(size_t oid, uint64_t line, uint64_t offset, const char *fileName, std::string content) : Operation(oid, line, offset, fileName, line, offset, fileName, content) {
    
  }

  Operation::Operation(size_t oid, uint64_t startLine, uint64_t startOffset, const char *startFileName, uint64_t endLine, uint64_t endOffset, const char *endFileName, std::string content) : oid(oid), startLine(startLine), startOffset(startOffset), startFileName(startFileName), endLine(endLine), endOffset(endOffset), endFileName(endFileName), content(content) {
    
  }

  void Operation::draw(std::shared_ptr<vobj::RootDisplay> root, sf::RenderTarget &t) {
    for (auto &comp : comps) {
      comp->draw(root, t);
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
