#pragma once

#include "vstd/base.hpp"
#include "vobj/all.hpp"
#include <vector>
#include <iostream>
#include <memory>

namespace vstd {
  template<typename T>
  class vector : public std::vector<T>, public vstd::base {
    using EBT = vobj::BackingType<T>::type;
    using Allocator = std::allocator<T>;

    // for now, assert that element is a primitive
    // TODO: handle nesting
    ASSERT_PRIMITIVE(T);

    std::shared_ptr<vobj::List<EBT>> bo;

    void init_helper(std::source_location sloc) {
      std::cout << "VEC INIT AT " << sloc.line() << std::endl;
      bo = vobj::create<vobj::List<EBT>>();
      MODEL.addOp(sloc, "vector initialization");
      vobj::Operation &op = MODEL.ops.back();
      op.comps.push_back(std::make_unique<vobj::ConstructOp>(bo));
      for (size_t i = 0; i < std::vector<T>::size(); ++i) {
        // create backing element object and add init op
        std::shared_ptr<EBT> e = vobj::create<EBT>(std::vector<T>::at(i));
        bo->elements[i] = e;
        op.comps.push_back(std::make_unique<vobj::ConstructOp>(e));
        op.comps.push_back(std::make_unique<vobj::MoveOp>(e, nullptr, bo));
        op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));
      }
      vcore::controller.spin();
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

    vector(SLOC) : std::vector<T>() {
      init_helper(sloc);
    }

    vector(const vector& other, SLOC) : std::vector<T>(other) {
      init_helper(sloc);
    }

    vector(vector&& other, SLOC) noexcept : std::vector<T>(std::move(other)) {
      init_helper(sloc);
    }

    vector(size_t count, SLOC) : std::vector<T>(count) {
      init_helper(sloc);
    }

    vector(size_t count, const T& value, SLOC) : std::vector<T>(count, value) {
      init_helper(sloc);
    }
  };
}