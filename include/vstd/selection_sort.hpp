#pragma once

#include "vobj/display.hpp"
#include <iterator>

namespace vstd {

template <typename RandomIt>
void selection_sort(RandomIt first, RandomIt last) {
  using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
  if (first == last)
    return;
  static_assert(std::random_access_iterator<RandomIt>,
                "vstd::selection_sort requires random access iterators");
  const diff_t n = last - first;
  for (diff_t i = 0; i < n - 1; ++i) {
    diff_t minIdx = i;
    for (diff_t j = i + 1; j < n; ++j) {
      if (*(first + j) < *(first + minIdx)) {
        minIdx = j;
      }
    }
    if (minIdx != i) {
      using std::iter_swap;
      iter_swap(first + i, first + minIdx);
      SLOC;
      UPDATE;
      SPIN;
    }
  }
}

} // namespace vstd
