#pragma once

#include "vobj/display.hpp" 
#include <iterator>

namespace vstd {

template <typename RandomIt>
void insertion_sort(RandomIt first, RandomIt last) {
  using std::iter_swap;
  using diff_t = typename std::iterator_traits<RandomIt>::difference_type;

  if (first == last)
    return;


  const diff_t n = last - first;
  for (diff_t i = 1; i < n; ++i) {
    auto key = *(first + i);
    diff_t j = i - 1;

    while (j >= 0 && *(first + j) > key) {
      *(first + j + 1) = *(first + j);


      SLOC;
      UPDATE;
      SPIN;

      --j;
    }

    *(first + j + 1) = key;

    SLOC;
    UPDATE;
    SPIN;
  }
}

} 
