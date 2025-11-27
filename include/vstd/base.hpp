#pragma once

#include "vobj/operation.hpp"

namespace vstd {
  class base {
    // check values to see what changed since last update, update vobj displays to reflect this
    // recursively call child elements
    // named this way to prevent collision with std:: member functions
    virtual bool _vstd_update_values(vobj::Operation &op) = 0;

    friend struct vobj::Display;
    friend vobj::RootDisplay;
  public:

    // give custom name to this object
    virtual void _vstd_rename(std::string name, SLOC) = 0;

    // get type name of this vstd object
    virtual std::string _vstd_type_name() const = 0;
  };
}