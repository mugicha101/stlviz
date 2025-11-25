#pragma once

// include all vstd classes
#include "vstd/base.hpp"
#include "vstd/vector.hpp"
#include "vstd/stack.hpp"
#include "vobj/backing_type.hpp"

// map vstd class to vobj class at compile time
namespace vobj {
  template<typename T>
  struct BackingType<vstd::vector<T>> {
    using type = vobj::List<std::shared_ptr<typename BackingType<T>::type>>;
  };

  template<typename T, typename Container>
  struct BackingType<vstd::stack<T, Container>> {
    using type = vobj::List<std::shared_ptr<typename BackingType<T>::type>>;
  };
}

// allow naming vstd objects to their variable name
#define DEF(X) X._vstd_rename(#X)

// allows main to keep window open after finishing
// does this by intercepting final destructor call
#define MAIN_DONE() CONTROLLER.mainDone = true
