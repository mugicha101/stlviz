#pragma once

#include "vobj/display.hpp"
#include <iterator>
#include <utility>

namespace vstd {

template <typename RandomIt> void heap_sort(RandomIt first, RandomIt last) {
  using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
  if (first == last)
    return;
  static_assert(std::random_access_iterator<RandomIt>,
                "vstd::heap_sort requires random access iterators");
  const diff_t n = last - first;
  auto sift_down = [&](diff_t root, diff_t count, auto &&self) -> void {
    while (true) {
      diff_t left = 2 * root + 1;
      if (left >= count)
        break;
      diff_t largest = root;
      if (*(first + left) > *(first + largest))
        largest = left;
      diff_t right = left + 1;
      if (right < count && *(first + right) > *(first + largest))
        largest = right;
      if (largest == root)
        break;
      using std::swap;
      swap(*(first + root), *(first + largest));
      SLOC;
      UPDATE;
      SPIN;
      root = largest;
    }
  };
  for (diff_t start = n / 2 - 1; start >= 0; --start) {
    sift_down(start, n, sift_down);
    if (start == 0)
      break;
  }
  for (diff_t end = n - 1; end > 0; --end) {
    using std::swap;
    swap(*first, *(first + end));
    SLOC;
    UPDATE;
    SPIN;
    sift_down(0, end, sift_down);
  }
}

} // namespace vstd
