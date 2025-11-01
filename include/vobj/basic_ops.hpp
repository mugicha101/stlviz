#pragma once

#include "vobj/operation.hpp"
#include "vobj/backing_type.hpp"
#include <iostream>

// define major operator component types
namespace vobj {
  // create a new object
  struct ConstructOp : public OpComp {
    std::shared_ptr<Display> target;
    ConstructOp(std::shared_ptr<Display> target) : target(target) {}
    void apply() override;
    void undo() override;
  };

  // destroy an object
  struct DestroyOp : public OpComp {
    std::shared_ptr<Display> target;
    DestroyOp(std::shared_ptr<Display> target) : target(target) {}
    void apply() override;
    void undo() override;
  };

  // modify an existing object, ex: change size or capacity of vector
  struct MutOp : public OpComp {};

  // change parent of backing object
  struct MoveOp : public OpComp {
    std::shared_ptr<Display> target;
    std::shared_ptr<Display> oldParent;
    std::shared_ptr<Display> newParent;
    MoveOp(std::shared_ptr<Display> target, std::shared_ptr<Display> oldParent, std::shared_ptr<Display> newParent);
    void apply() override;
    void undo() override;
  };

  // value of type T updated, T must have a backing type of Primitive<T>
  template<typename T>
  struct AssignOp : public OpComp {
    using BT = BackingType<T>::type;
    static_assert(std::is_same_v<BT, Primitive<T>>, "AssignOp<T>: T must be backed by Primitive<T>");

    std::shared_ptr<BT> target;
    T oldValue;
    T newValue;

    AssignOp(std::shared_ptr<BT> target, T oldValue, T newValue) : target(target), oldValue(oldValue), newValue(newValue) {}

    void apply() override {
      target->value = newValue;
    }

    void undo() override {
      target->value = oldValue;
    }
  };
}