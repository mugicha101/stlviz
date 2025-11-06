#include "vobj/basic_ops.hpp"

namespace vobj {
  void ConstructOp::apply() {
    std::cout << target->uid << " ALIVE" << std::endl;
    target->alive = true;
    target->name = defaultName;
  }

  void ConstructOp::undo() {
    std::cout << target->uid << " DEAD" << std::endl;
    target->alive = false;
    target->name = "";
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
    std::cout << target->uid << " P " << (target->parent ? (int)target->parent->uid : -1) << std::endl;
  }
  void MoveOp::undo() {
    target->parent = oldParent;
    std::cout << target->uid << " P " << (target->parent ? (int)target->parent->uid : -1) << std::endl;
  }

  void RenameOp::apply() {
    target->name = newName;
  }
  void RenameOp::undo() {
    target->name = oldName;
  }
}