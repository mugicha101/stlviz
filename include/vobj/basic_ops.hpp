#pragma once

#include "vobj/operation.hpp"
#include "vobj/backing_type.hpp"
#include <iostream>
#include <string>

// define major operator component types
namespace vobj {
  std::string genDefaultName(std::shared_ptr<vobj::Display> target, std::source_location sloc);

  // create a new object
  struct ConstructOp : public OpComp {
    std::shared_ptr<Display> target;
    std::string defaultName;
    ConstructOp(std::shared_ptr<Display> target, std::source_location sloc) : target(target), defaultName(genDefaultName(target, sloc)) {}
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

    void draw(std::shared_ptr<vobj::RootDisplay> root, sf::RenderTarget &t) override {
      sf::IntRect bbox = target->getBBox();
      std::vector<sf::Vector2f> drawLocs;
      target->getGlobalDrawLocs(drawLocs);
      for (sf::Vector2f &pos : drawLocs) {
        sf::RectangleShape rect(static_cast<sf::Vector2f>(bbox.size) / root->camZoom);
        sf::Vector2f screenPos = root->world2screen(pos);
        // Account for tab bar offset - data view is drawn below the tab bar
        screenPos.y += 40.f; // tabBarHeight
        rect.setPosition(screenPos);
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Red);
        rect.setOutlineThickness(3.f * (1.f / root->camZoom));
        t.draw(rect);
      }
    }
  };

  // rename an object
  struct RenameOp : public OpComp {
    std::shared_ptr<Display> target;
    std::string oldName;
    std::string newName;

    RenameOp(std::shared_ptr<Display> target, std::string oldName, std::string newName) : target(target), oldName(oldName), newName(newName) {}

    void apply() override;

    void undo() override;
  };
}