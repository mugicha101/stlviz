#include "vobj/operation.hpp"

namespace vobj {
  Operation::Operation(uint64_t line, uint64_t offset, std::string content) : Display(), line(line), offset(offset), content(content) {
    
  }

  void Operation::draw(sf::RenderTarget &c, sf::Transform t) {
    for (auto &comp : comps) {
      comp->draw(c, t);
    }
  }

  void Operation::apply() {
    for (auto &comp : comps) {
      comp->apply();
    }
  }

  void Operation::undo() {
    for (auto it = comps.rbegin(); it != comps.rend(); ++it) {
      (*it)->undo();
    }
  }
}
