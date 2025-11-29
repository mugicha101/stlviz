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
#include "vstd/primitive_wrappers.hpp"
#include "vstd/set.hpp"
#include "vstd/bitset.hpp"

// allows naming vstd objects to their variable name
#define DEF(X) X._vstd_rename(#X)

// allows naming vstd objects to an arbitrary name
#define DEFN(X, NAME) X._vstd_rename(#NAME)

// allows main to keep window open after finishing
// does this by intercepting final destructor call
#define MAIN_DONE CONTROLLER.mainDone = true

// allows user to specify a point to capture updated values
#define SNAP CONTROLLER.update(std::source_location::current())