#pragma once

#include "vobj/operation.hpp"
#include "vobj/backing_type.hpp"

// define major operator component types
namespace vobj {
  // create a new object
  struct ConstructOp : public OpComp {
    std::shared_ptr<Display> target;
    ConstructOp(std::shared_ptr<Display> target) : target(target) {}
    void apply() override;
    void undo() override;
    void draw(sf::RenderTarget &c, sf::Transform t) override;
  };

  // destroy an object
  struct DestroyOp : public OpComp {
    std::shared_ptr<Display> target;
    DestroyOp(std::shared_ptr<Display> target) : target(target) {}
    void apply() override;
    void undo() override;
    void draw(sf::RenderTarget &c, sf::Transform t) override;
  };

  // modify an existing object, ex: change size or capacity of vector
  struct MutOp : public OpComp {};

  // value of type T updated, T must have a backing type of Primitive<T>
  template<typename T>
  struct AssignOp : public OpComp {
    using BT = BackingType<T>::type;
    static_assert(std::is_same_v<BT, typename Primitive<T>::type>, "AssignOp<T>: T must be backed by Primitive<T>");

    std::shared_ptr<BT> target;
    BT oldValue;
    BT newValue;

    AssignOp(std::shared_ptr<BT> target, BT oldValue, BT newValue) : target(target), oldValue(oldValue), newValue(newValue) {}

    void apply() override {
      target->value = newValue;
    }

    void undo() override {
      target->value = oldValue;
    }
    
    void draw(sf::RenderTarget &c, sf::Transform t) override {
      
    }
  };
}