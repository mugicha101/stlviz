#pragma once

// include all vstd classes
#include "vstd/base.hpp"
#include "vstd/vector.hpp"
#include "vobj/backing_type.hpp"

// map vstd class to vobj class at compile time
namespace vobj {
  template<typename T>
  struct BackingType<vstd::vector<T>> {
    using type = vobj::List<std::shared_ptr<typename BackingType<T>::type>>;
  };
}

// allow naming vstd objects to their variable name
#define DEF(X) X._vstd_rename(#X)