#include "vobj/basic_ops.hpp"

namespace vobj {
  void ConstructOp::apply() {
    target->setAlive(true);
    target->name = defaultName;
  }

  void ConstructOp::undo() {
    target->setAlive(false);
    target->name = "";
  }

  void DestroyOp::apply() {
    target->setAlive(false);
  }

  void DestroyOp::undo() {
    target->setAlive(true);
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