#pragma once

#include "vobj/primitive.hpp"
#include <type_traits>

#define ASSERT_PRIMITIVE(T) static_assert(std::is_same_v<vobj::BackingType<T>::type, vobj::Primitive<T>>, "T does not having a backing type of vobj::Primitive<T>");

// map vstd / primitive classes to backing vobj class
// implemented for each vstd class in vstd/all.hpp
namespace vobj {
  template<typename T>
  struct BackingType {
    using type = vobj::Primitive<T>;
  };
}
