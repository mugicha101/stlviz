#pragma once

#include "vstd/base.hpp"
#include "vobj/all.hpp"
#include <vector>
#include <iostream>
#include <memory>

namespace vstd {
  template<typename T>
  class vector : public std::vector<T>, public base {
    using EBT = vobj::BackingType<T>::type;
    // using Allocator = std::allocator<T>; TODO: support custom allocators like std::vector does
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;

    // for now, assert that element is a primitive
    // TODO: handle nesting
    ASSERT_PRIMITIVE(T);

    std::shared_ptr<vobj::List<EBT>> bo;

    bool _vstd_update_values(vobj::Operation &op) override {
      std::cout << "VEC UPDATE" << std::endl;
      bool ret = false;

      for (size_t i = 0; i < size(); ++i) {
        if (bo->elements.find(i) == bo->elements.end()) {
          throw std::runtime_error("vector<T>::_vstd_update_values: backing object missing element at index " + std::to_string(i));
        }
        std::shared_ptr<EBT> e = bo->elements[i];
        if constexpr (IS_PRIMITIVE(EBT)) {
          ret = ret | std::dynamic_pointer_cast<EBT>(e)->update(op, std::vector<T>::at(i));
        } else {
          // TODO: handle nested objects properly (this part shouldn't get called)
          std::runtime_error("vector<T>::_vstd_update_values: nested vstd objects not yet supported!");
          // ret = ret | e->update(op);
        }
      }
      return ret;
    }

    void init_helper(std::source_location sloc) {
      std::cout << "VEC INIT AT " << sloc.line() << std::endl;
      OP("vector initialization",
        bo = vobj::create<vobj::List<EBT>>();
        bo->o = (vstd::base *)this;
        std::cout << "BO UID " << bo->uid << " MAPPED TO VEC " << std::endl;
        op.comps.push_back(std::make_unique<vobj::ConstructOp>(bo, sloc));
        for (size_t i = 0; i < size(); ++i) {
          // create backing element object and add init op
          std::shared_ptr<EBT> e = vobj::create<EBT>(std::vector<T>::at(i));
          bo->add(op, i, e);
          op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));
        }
      )
    }

    void push_back_helper(vobj::Operation &op) {
      size_t i = size() - 1;
      std::shared_ptr<EBT> e = vobj::create<EBT>(std::vector<T>::at(i));
      bo->add(op, i, e);
      op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));
    }

  public:

    void _vstd_rename(std::string name, SLOC) override {
      OP("rename vector",
        op.comps.push_back(std::make_unique<vobj::RenameOp>(bo, bo->name, name));
      )
    }

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

    ~vector() {;
      SLOC;
      OP("vector destruction",
        for (size_t i = 0; i < size(); ++i) {
          bo->remove(op, i);
        }
        op.comps.push_back(std::make_unique<vobj::DestroyOp>(bo));
      )
    }

    // since iterators can only update values, and vast majority of usecases use auto for type resolution, can just use iterator directly to std::vector
    iterator begin() { return std::vector<T>::begin(); }
    iterator end()   { return std::vector<T>::end(); }
    const_iterator begin() const { return std::vector<T>::begin(); }
    const_iterator end() const { return std::vector<T>::end(); }

    size_t size() const { return std::vector<T>::size(); }

    void push_back(const T &value, SLOC) {
      OP("vector push_back",
        std::vector<T>::push_back(value);
        push_back_helper(op);
      )
    }

    void push_back(T &&value, SLOC) {
      OP("vector push_back",
        std::vector<T>::push_back(std::move(value));
        push_back_helper(op);
      )
    }

    void pop_back(SLOC) {
      OP("vector pop_back",
        std::vector<T>::pop_back();
        size_t i = size();
        bo->remove(op, i);
      )
    }
  };
}