#pragma once

// include all vstd classes
#include "vstd/base.hpp"
#include "vstd/vector.hpp"
#include "vstd/stack.hpp"
#include "vstd/deque.hpp"
#include "vstd/binary_tree.hpp"
#include "vstd/binary_search_tree.hpp"
#include "vobj/backing_type.hpp"
#include "vstd/insertion_sort.hpp"
#include "vstd/selection_sort.hpp"
#include "vstd/bubble_sort.hpp"
#include "vstd/merge_sort.hpp"
#include "vstd/heap_sort.hpp"
#include "vstd/quick_sort.hpp"
#include "vstd/set.hpp"

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
  template <typename T>
  struct BackingType<vstd::deque<T>> {
    using type = vobj::List<std::shared_ptr<typename BackingType<T>::type>>;
  };

  template <typename T, typename Compare>
  struct BackingType<vstd::set<T, Compare>> {
    using type = vobj::List<std::shared_ptr<typename BackingType<T>::type>>;
  };
  
  template<typename T>
  struct BackingType<vstd::binary_tree<T>> {
    using type = vobj::Tree<std::shared_ptr<typename BackingType<T>::type>>;
  };

  template<typename T>
  struct BackingType<vstd::binary_search_tree<T>> {
    using type = vobj::Tree<std::shared_ptr<typename BackingType<T>::type>>;
  };
}

// allow naming vstd objects to their variable name
#define DEF(X) X._vstd_rename(#X)


// allows main to keep window open after finishing
// does this by intercepting final destructor call
#define MAIN_DONE() CONTROLLER.mainDone = true
