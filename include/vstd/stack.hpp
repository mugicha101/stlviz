#pragma once

#include "vstd/base.hpp"
#include "vobj/all.hpp"
#include <stack>
#include <iostream>
#include <memory>
#include <deque>

namespace vstd {
  template<typename T, typename Container = std::deque<T>>
  class stack : public std::stack<T, Container>, public base {
    using SUPER = std::stack<T, Container>;
    using EBT = vobj::BackingType<T>::type;
    using std::stack<T, Container>::c;

    // assert that element is a primitive
    ASSERT_PRIMITIVE(T);

    std::shared_ptr<vobj::List<EBT>> bo;

    bool _vstd_update_values(vobj::Operation &op) override {
      std::cout << "STACK UPDATE" << std::endl;
      bool ret = false;

      // need to track the container's elements, use backing object
      size_t i = 0;
      for (T &x : c) {
        if (bo->elements.find(i) == bo->elements.end()) {
          ERR("stack<T>::_vstd_update_values: backing object missing element at index " + std::to_string(i));
        }
        std::shared_ptr<EBT> e = bo->elements[i];
        if constexpr (IS_PRIMITIVE(EBT)) {
          ret = ret | std::dynamic_pointer_cast<EBT>(e)->update(op, x);
        } else {
          // TODO: handle nested objects properly (this part shouldn't get called)
          ERR("stack<T>::_vstd_update_values: nested vstd objects not yet supported!");
          // ret = ret | e->update(op);
        }
        i++;
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

    std::string _vstd_type_name() const override {
      return "stack";
    }

    // Constructors
    stack(SLOC) {
      init_helper(sloc);
    }

    stack(const stack& other, SLOC) : SUPER(other.s) {
      init_helper(sloc);
    }

    stack(stack&& other, SLOC) noexcept : SUPER(std::move(other.s)) {
      init_helper(sloc);
    }

    ~stack() {
      SLOC;
      OP("stack destruction",
        for (size_t i = 0; i < size(); ++i) {
          bo->remove(op, i);
        }
        op.comps.push_back(std::make_unique<vobj::DestroyOp>(bo));
      )
    }

    // Stack operations
    void push(const T &value, SLOC) {
      OP("stack push",
        SUPER::push(value);
        size_t i = size() - 1;
        std::shared_ptr<EBT> e = vobj::create<EBT>(value);
        bo->add(op, i, e);
        op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));
      )
    }

    void push(T &&value, SLOC) {
      OP("stack push",
        SUPER::push(std::move(value));
        size_t i = size() - 1;
        std::shared_ptr<EBT> e = vobj::create<EBT>(top());
        bo->add(op, i, e);
        op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));
      )
    }

    void pop(SLOC) {
      OP("stack pop",
        if (!empty()) {
          SUPER::pop();
          size_t i = size();
          bo->remove(op, i);
        }
      )
    }

    T& top() {
      return SUPER::top();
    }

    const T& top() const {
      return SUPER::top();
    }

    bool empty() const {
      return SUPER::empty();
    }

    size_t size() const {
      return SUPER::size();
    }
  };
}
