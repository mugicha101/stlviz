#pragma once

#include <SFML/Graphics.hpp>
#include <deque>
#include <source_location>
#include "vobj/draw_dep.hpp"

#define SLOC std::source_location sloc = std::source_location::current()
#define CONTROLLER vcore::controller
#define MODEL vcore::controller.model
#define VIEW vcore::controller.view
#define SPIN vcore::controller.spin();
#define UPDATE vcore::controller.update(sloc);
#define OP(content, body) UPDATE; vobj::Operation &op = MODEL.addOp(sloc, content); body; SPIN
#define FONT_SIZE 32
#define OP_FONT_SIZE 16
#define ERR(msg) std::cerr << msg << std::endl; throw std::runtime_error(msg)

#define FRIEND_CREATE template<typename T_FRIEND, typename... Args> friend std::shared_ptr<T_FRIEND> create(Args&&... args);

namespace vstd {
  class base;
}

namespace vobj {
  struct Operation;
  struct Display;

  // represent an object to be displayed on the ui, should not use copy/move constructors (use shared_ptr to pass around)
  // since operation can be reverse, once created should never be destroyed during lifetime of program
  // all values should be updated only by the operation
  struct Display : public std::enable_shared_from_this<Display> {
    // stores all displays with index being uid
    static std::deque<std::shared_ptr<Display>> displays;
    
    static sf::Font &getFont();

    static uint64_t globalDrawTick; // the current draw tick to allow rendering mark an object as drawn this tick (incremented by View)
    static uint64_t globalUpdateTick; // the current update tick to allow updating mark an object as updated this tick (incremented by Model)
    bool updated = false; // cached result of this tick's update call
    sf::Vector2f rootPos; // position of element when it has no parent

    size_t uid = SIZE_MAX;

    std::string name; // name (set to init location by ConstructOp, can be modified with RenameOp)

    sf::RenderTexture canvas; // sfml canvas that caches this object's drawing (parents can use it)
    
    sf::IntRect bbox; // bounding box in canvas (so don't have to draw entire canvas)

    vstd::base *o; // pointer to vstd object this display represents, nullptr if none (raw pointer due to vstd not being created as a smart ptr)

    std::shared_ptr<Display> parent; // pointer to parent display, nullptr if none

    bool alive = false; // true if the object is alive by the current operation's completion

    uint64_t localDrawTick = 0; // if matches globalDrawTick this object has been rendered this tick
    uint64_t localUpdateTick = 0; // if matches globalUpdateTick this object has been updated this tick

    std::deque<DrawDep> drawDeps; // draw locations, updated by drawOn calls

  protected:

    // hide constructor so forced to use create
    Display();

    virtual ~Display() = default;

    FRIEND_CREATE

  public:

    // rename this display, should only be done when at latest operation
    void rename(std::string name, std::source_location sloc);

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
    // should update the drawn locations of this object this tick for ops to use
    virtual void drawOn(sf::RenderTarget &t, float x, float y, std::shared_ptr<Display> context = nullptr);
    
    // compares value with mapped vstd class instance, if value changed since last update, adds value change to op and returns true
    // relies on vstd::base::_vstd_update_values() to do all the heavy lifting since raw type is unknown here
    // similar to visitor design pattern
    virtual bool update(Operation &op);

    // update local canvas
    virtual void draw();

    // gets positions in global coordinates this object was drawn at this tick
    // does this by following DrawDep DAG
    // appends to res
    // recursive (adds relative to offset)
    void getGlobalDrawLocs(std::vector<sf::Vector2f> &res, sf::Vector2f offset = {0,0}) const;
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
