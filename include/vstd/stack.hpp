#pragma once

#include "vstd/base.hpp"
#include "vobj/all.hpp"
#include <stack>
#include <iostream>
#include <memory>
#include <deque>

namespace vstd {
  template<typename T, typename Container = std::deque<T>>
  class stack : public base {
    using EBT = vobj::BackingType<T>::type;

    // assert that element is a primitive
    ASSERT_PRIMITIVE(T);

    std::stack<T, Container> s;
    std::shared_ptr<vobj::List<EBT>> bo;

    bool _vstd_update_values(vobj::Operation &op) override {
      std::cout << "STACK UPDATE" << std::endl;
      bool ret = false;

      // need to track the container's elements, use backing object
      size_t idx = 0;
      for (auto& elem : bo->elements) {
        idx++;
      }
      return ret;
    }

    void init_helper(std::source_location sloc) {
      std::cout << "STACK INIT AT " << sloc.line() << std::endl;
      OP("stack initialization",
        bo = vobj::create<vobj::List<EBT>>();
        bo->o = (vstd::base *)this;
        std::cout << "BO UID " << bo->uid << " MAPPED TO STACK " << std::endl;
        op.comps.push_back(std::make_unique<vobj::ConstructOp>(bo, sloc));
      )
    }

  public:

    void _vstd_rename(std::string name, SLOC) override {
      OP("rename stack",
        op.comps.push_back(std::make_unique<vobj::RenameOp>(bo, bo->name, name));
      )
    }

    // Constructors
    stack(SLOC) {
      init_helper(sloc);
    }

    stack(const stack& other, SLOC) : s(other.s) {
      init_helper(sloc);
    }

    stack(stack&& other, SLOC) noexcept : s(std::move(other.s)) {
      init_helper(sloc);
    }

    ~stack() {
      SLOC;
      OP("stack destruction",
        // Remove all elements from the backing object
        while (!bo->elements.empty()) {
          bo->remove(op, bo->elements.size() - 1);
        }
        op.comps.push_back(std::make_unique<vobj::DestroyOp>(bo));
      )
    }

    // Stack operations
    void push(const T &value, SLOC) {
      OP("stack push",
        s.push(value);
        size_t i = size() - 1;
        std::shared_ptr<EBT> e = vobj::create<EBT>(value);
        bo->add(op, i, e);
        op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));
      )
    }

    void push(T &&value, SLOC) {
      OP("stack push",
        s.push(std::move(value));
        size_t i = size() - 1;
        std::shared_ptr<EBT> e = vobj::create<EBT>(s.top());
        bo->add(op, i, e);
        op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));
      )
    }

    void pop(SLOC) {
      OP("stack pop",
        if (!s.empty()) {
          s.pop();
          size_t i = size();
          bo->remove(op, i);
        }
      )
    }

    T& top() {
      return s.top();
    }

    const T& top() const {
      return s.top();
    }

    bool empty() const {
      return s.empty();
    }

    size_t size() const {
      return s.size();
    }
  };
}
