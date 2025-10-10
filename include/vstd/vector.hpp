#pragma once

#include "vstd/base.hpp"
#include "vobj/all.hpp"
#include <vector>
#include <iostream>
#include <memory>

namespace vstd {
  template<typename T>
  class vector : public std::vector<T>, public vstd::base {
    using std::vector<T>::vector;
    using EBT = vobj::BackingType<T>::type;

    // for now, assert that element is a primitive
    // TODO: handle nesting
    ASSERT_PRIMITIVE(T);

    std::shared_ptr<vobj::List<EBT>> bo;

    void init_helper() {
      bo = vobj::create<vobj::List<EBT>>();
      // vobj::Operation op;
      // TODO: add init op + resize op
      // for (size_t i = 0; i < std::vector<T>::size(); ++i) {
      //   TODO: add element i to model
      // }
    }

    bool _vstd_update_values(vobj::Operation &op) override {
      auto &elements = bo->elements;
      bool ret = false;
      for (size_t i = 0; i < std::vector<T>::size(); ++i) {
        std::shared_ptr<EBT> e = elements[i];
        e->latest = std::vector<T>::at(i);
        ret = ret | e->update(op);
      }
      return false;
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
  };
}