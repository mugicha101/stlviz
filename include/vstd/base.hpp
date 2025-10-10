#pragma once

#include "vobj/operation.hpp"

namespace vstd {
  class base {
    // check values to see what changed since last update, update vobj displays to reflect this
    // recursively call child elements
    // named this way to prevent collision with std:: member functions
    virtual bool _vstd_update_values(vobj::Operation &op) = 0;

    friend struct vobj::Display;
  };
}