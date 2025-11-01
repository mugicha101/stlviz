#include "vobj/basic_ops.hpp"

namespace vobj {
  void ConstructOp::apply() {
    target->alive = true;
  }

  void ConstructOp::undo() {
    target->alive = false;
  }

  void DestroyOp::apply() {
    target->alive = false;
  }

  void DestroyOp::undo() {
    target->alive = true;
  }

  MoveOp::MoveOp(std::shared_ptr<Display> target, std::shared_ptr<Display> oldParent, std::shared_ptr<Display> newParent) : target(target), oldParent(oldParent), newParent(newParent) {

  }
  void MoveOp::apply() {
    target->parent = newParent;
  }
  void MoveOp::undo() {
    target->parent = oldParent;
  }
}