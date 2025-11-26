#pragma once

#include "vobj/display.hpp"
#include <iterator>

namespace vstd {

template <typename RandomIt> void bubble_sort(RandomIt first, RandomIt last) {
  using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
  if (first == last)
    return;
  static_assert(std::random_access_iterator<RandomIt>,
                "vstd::bubble_sort requires random access iterators");
  const diff_t n = last - first;
  for (diff_t i = 0; i < n - 1; ++i) {
    bool swapped = false;
    for (diff_t j = 0; j < n - 1 - i; ++j) {
      if (*(first + j) > *(first + j + 1)) {
        using std::iter_swap;
        iter_swap(first + j, first + j + 1);
        SLOC;
        UPDATE;
        SPIN;
        swapped = true;
      }
    }
    if (!swapped)
      break;
  }
}

} // namespace vstd
