#pragma once

#include "vobj/display.hpp"
#include <iterator>
#include <utility>

namespace vstd {

template <typename RandomIt> void quick_sort(RandomIt first, RandomIt last) {
  using diff_t = typename std::iterator_traits<RandomIt>::difference_type;
  if (first == last)
    return;
  static_assert(std::random_access_iterator<RandomIt>,
                "vstd::quick_sort requires random access iterators");
  auto part = [&](diff_t l, diff_t r) -> diff_t {
    auto pivot = *(first + r);
    diff_t i = l;
    for (diff_t j = l; j < r; ++j) {
      if (*(first + j) <= pivot) {
        using std::swap;
        swap(*(first + i), *(first + j));
        SLOC;
        UPDATE;
        SPIN;
        ++i;
      }
    }
    using std::swap;
    swap(*(first + i), *(first + r));
    SLOC;
    UPDATE;
    SPIN;
    return i;
  };
  auto rec = [&](auto &&self, diff_t l, diff_t r) -> void {
    if (l >= r)
      return;
    diff_t p = part(l, r);
    if (p > 0)
      self(self, l, p - 1);
    self(self, p + 1, r);
  };
  diff_t n = last - first;
  if (n > 0)
    rec(rec, 0, n - 1);
}

} // namespace vstd
