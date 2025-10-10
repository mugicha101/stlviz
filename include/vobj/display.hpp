#pragma once

#include <SFML/Graphics.hpp>
#include <deque>

#define FRIEND_CREATE template<typename T, typename... Args> friend std::shared_ptr<T> create(Args&&... args);

namespace vstd {
  class base;
}

namespace vobj {
  struct Operation;

  // represent an object to be displayed on the ui, should not use copy/move constructors (use shared_ptr to pass around)
  // since operation can be reverse, once created should never be destroyed during lifetime of program
  // all values should be updated only by the operation
  struct Display {
    // stores all displays with index being uid
    static std::deque<std::shared_ptr<Display>> displays;
    
    size_t uid = SIZE_MAX;
    sf::FloatRect bbox; // bounding box in global coordinates

    std::shared_ptr<vstd::base> o; // pointer to vstd object this display represents, nullptr if none

    std::shared_ptr<Display> parent; // pointer to parent display, nullptr if none

    bool alive = false; // true if the object is alive by the current operation's completion

  protected:

    // hide constructor so forced to use create
    Display();

    FRIEND_CREATE

  public:
    
    // compares value with mapped vstd class instance, if value changed since last update, adds value change to op and returns true
    // relies on vstd::base::_vstd_update_values() to do all the heavy lifting since raw type is unknown here
    // similar to visitor design pattern
    virtual bool update(Operation &op);

    // draw object on c with global transform t, also updates bounding box based on t
    virtual void draw(sf::RenderTarget &c, sf::Transform t) = 0;
  };

  // factory
  template<typename T, typename... Args>
  std::shared_ptr<T> create(Args&&... args) {
    static_assert(std::is_base_of_v<Display, T>, "create<T>: T must be derived from Display");
    auto ret = std::shared_ptr<T>(new T(std::forward<Args>(args)...));
    ret->uid = Display::displays.size();
    Display::displays.push_back(ret);
    return ret;
  }
}
