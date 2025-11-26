#pragma once

#include "vobj/display.hpp"
#include <iterator>
#include <vector>

namespace vstd {

template <typename RandomIt> void merge_sort(RandomIt first, RandomIt last) {
  using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
  using T = typename std::iterator_traits<RandomIt>::value_type;
  if (first == last)
    return;
  static_assert(std::random_access_iterator<RandomIt>,
                "vstd::merge_sort requires random access iterators");
  const diff_t n = last - first;
  std::vector<T> tmp(n);
  auto rec = [&](auto &&self, diff_t l, diff_t r) -> void {
    if (r - l <= 1)
      return;
    diff_t m = l + (r - l) / 2;
    self(self, l, m);
    self(self, m, r);
    diff_t i = l;
    diff_t j = m;
    diff_t k = l;
    while (i < m && j < r) {
      if (*(first + j) < *(first + i)) {
        tmp[k - l] = *(first + j);
        ++j;
      } else {
        tmp[k - l] = *(first + i);
        ++i;
      }
      ++k;
    }
    while (i < m) {
      tmp[k - l] = *(first + i);
      ++i;
      ++k;
    }
    while (j < r) {
      tmp[k - l] = *(first + j);
      ++j;
      ++k;
    }
    for (diff_t x = l; x < r; ++x) {
      *(first + x) = tmp[x - l];
      SLOC;
      UPDATE;
      SPIN;
    }
  };
  rec(rec, 0, n);
}

} // namespace vstd
