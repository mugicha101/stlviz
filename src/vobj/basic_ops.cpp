#include "vobj/basic_ops.hpp"

namespace vobj {
  void ConstructOp::apply() {
    target->alive = true;
  }

  void ConstructOp::undo() {
    target->alive = false;
  }

  void ConstructOp::draw(sf::RenderTarget &c, sf::Transform t) {
    // TODO
  }

  void DestroyOp::apply() {
    target->alive = false;
  }

  void DestroyOp::undo() {
    target->alive = true;
  }

  void DestroyOp::draw(sf::RenderTarget &c, sf::Transform t) {
    // TODO
  }
}