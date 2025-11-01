#pragma once

#include <SFML/Graphics.hpp>
#include <deque>
#include <source_location>

#define SLOC std::source_location sloc = std::source_location::current()
#define CONTROLLER vcore::controller
#define MODEL vcore::controller.model
#define VIEW vcore::controller.view

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
    
    static sf::Font &getFont();

    static uint64_t globalTick; // the current draw tick to allow rendering mark an object as drawn this tick

    // render all displays onto c
    // each call to this represents a tick (increments globalTick)
    static void drawAll(sf::RenderTarget &c);
    
    size_t uid = SIZE_MAX;

    sf::RenderTexture canvas; // sfml canvas that caches this object's drawing (parents can use it)
    
    sf::IntRect bbox; // bounding box in canvas (so don't have to draw entire canvas)

    std::shared_ptr<vstd::base> o; // pointer to vstd object this display represents, nullptr if none

    std::shared_ptr<Display> parent; // pointer to parent display, nullptr if none

    bool alive = false; // true if the object is alive by the current operation's completion

    uint64_t localTick = 0; // if localTick == globalTick this object has been rendered this tick

  protected:

    // hide constructor so forced to use create
    Display();

    FRIEND_CREATE

  public:

    // clear canvas and resizes canvas to be at minimum this size
    // should be called at start of draw()
    void resetCanvas(uint32_t width, uint32_t height);

    // get canvas texture
    // calls draw if not drawn this tick
    const sf::Texture &getTexture();

    // get bounding box
    // calls draw if not drawn this tick
    sf::IntRect getBBox();

    // draw canvas on another target
    // calls draw if not drawn this tick
    void drawOn(sf::RenderTarget &t, int x, int y);
    
    // compares value with mapped vstd class instance, if value changed since last update, adds value change to op and returns true
    // relies on vstd::base::_vstd_update_values() to do all the heavy lifting since raw type is unknown here
    // similar to visitor design pattern
    virtual bool update(Operation &op);

    // update local canvas
    virtual void draw();
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
