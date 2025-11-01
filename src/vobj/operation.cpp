#include "vobj/operation.hpp"
#include <iostream>

namespace vobj {
  Operation::Operation(size_t oid, uint64_t line, uint64_t offset, std::string content) : Display(), oid(oid), line(line), offset(offset), content(content) {
    
  }

  void Operation::draw() {
    if (localTick == globalTick) return;
    localTick = globalTick;

    for (auto &comp : comps) {
      comp->draw();
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
