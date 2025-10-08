#pragma once

#include "vcore/all.hpp"
#include <vector>
#include <iostream>

namespace vstd {
  template<typename T>
  class vector : public std::vector<T> {
    using std::vector<T>::vector;

    #if STLVIZ_ENABLED

    void init_helper() {
      std::cout << std::vector<T>::size() << std::endl;
      volatile int x = vcore::controller.currOp;
      // for (size_t i = 0; i < std::vector<T>::size(); ++i) {
      //   TODO: add element i to model
      // }
    }

public:

    vector() : std::vector<T>() {
      init_helper();
    }

    vector(const vector& other) : std::vector<T>(other) {
      init_helper();
    }

    vector(vector&& other) noexcept : std::vector<T>(std::move(other)) {
      init_helper();
    }

    #endif
  };
}