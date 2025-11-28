#include "vobj/operation.hpp"
#include <iostream>

namespace vobj {
  void OpComp::draw(std::shared_ptr<vobj::RootDisplay> root, sf::RenderTarget &t) {}

  Operation::Operation() : Operation(0, 0, {}, {}) {}

  Operation::Operation(size_t oid, std::string content, LocInfo loc) : Operation(oid, content, loc, loc) {}

  Operation::Operation(size_t oid, std::string content, LocInfo startLoc, LocInfo endLoc) : oid(oid), content(content), startLoc(startLoc), endLoc(endLoc) {
    
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
