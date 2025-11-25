#pragma once

#include "vobj/all.hpp"
#include "vstd/base.hpp"
#include <algorithm>
#include <deque>
#include <iostream>
#include <memory>
#include <vector>

namespace vstd {
template <typename T> class deque : public std::deque<T>, public base {
  using SUPER = std::deque<T>;
  using EBT = vobj::BackingType<T>::type;
  using iterator = typename SUPER::iterator;
  using const_iterator = typename SUPER::const_iterator;


  ASSERT_PRIMITIVE(T);

  std::shared_ptr<vobj::List<EBT>> bo;

  bool _vstd_update_values(vobj::Operation &op) override {
    std::cout << "DEQUE UPDATE" << std::endl;
    bool ret = false;

    for (size_t i = 0; i < size(); ++i) {
      if (bo->elements.find(i) == bo->elements.end()) {
        ERR("deque<T>::_vstd_update_values: backing object missing element at "
            "index " +
            std::to_string(i));
      }
      std::shared_ptr<EBT> e = bo->elements[i];
      if constexpr (IS_PRIMITIVE(EBT)) {
        ret = ret | std::dynamic_pointer_cast<EBT>(e)->update(op, SUPER::at(i));
      } else {

        ERR("deque<T>::_vstd_update_values: nested vstd objects not yet "
            "supported!");
      }
    }
    return ret;
  }

  void init_helper(std::source_location sloc) {
    std::cout << "DEQUE INIT AT " << sloc.line() << std::endl;
    OP(
        "deque initialization", bo = vobj::create<vobj::List<EBT>>();
        bo->o = (vstd::base *)this;
        std::cout << "BO UID " << bo->uid << " MAPPED TO DEQUE " << std::endl;
        op.comps.push_back(std::make_unique<vobj::ConstructOp>(bo, sloc));
        for (size_t i = 0; i < size(); ++i) {
          std::shared_ptr<EBT> e = vobj::create<EBT>(SUPER::at(i));
          bo->add(op, i, e);
          op.comps.push_back(
              std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));
        })
  }

  void push_back_helper(vobj::Operation &op) {
    size_t i = size() - 1;
    std::shared_ptr<EBT> e = vobj::create<EBT>(SUPER::at(i));
    bo->add(op, i, e);
    op.comps.push_back(
        std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));
  }

public:
  void _vstd_rename(std::string name, SLOC) override {
    OP("rename deque", op.comps.push_back(std::make_unique<vobj::RenameOp>(
                           bo, bo->name, name));)
  }


  deque(SLOC) : SUPER() { init_helper(sloc); }

  deque(const deque &other, SLOC) : SUPER(other) { init_helper(sloc); }

  deque(deque &&other, SLOC) noexcept : SUPER(std::move(other)) {
    init_helper(sloc);
  }

  deque(size_t count, SLOC) : SUPER(count) { init_helper(sloc); }

  deque(size_t count, const T &value, SLOC) : SUPER(count, value) {
    init_helper(sloc);
  }

  ~deque() {
    SLOC;
    OP(
        "deque destruction", for (size_t i = 0; i < size(); ++i) {
          bo->remove(op, i);
        } op.comps.push_back(std::make_unique<vobj::DestroyOp>(bo));)
  }


  iterator begin() { return SUPER::begin(); }
  iterator end() { return SUPER::end(); }
  const_iterator begin() const { return SUPER::begin(); }
  const_iterator end() const { return SUPER::end(); }

  size_t size() const { return SUPER::size(); }


  void push_back(const T &value, SLOC) {
    OP("deque push_back", SUPER::push_back(value); push_back_helper(op);)
  }

  void push_back(T &&value, SLOC) {
    OP("deque push_back", SUPER::push_back(std::move(value));
       push_back_helper(op);)
  }

  void pop_back(SLOC) {
    OP("deque pop_back", SUPER::pop_back(); size_t i = size();
       bo->remove(op, i);)
  }

  void push_front(const T &value, SLOC) {

    UPDATE;
    vobj::Operation &op = MODEL.addOp(sloc, "deque push_front");

    SUPER::push_front(value);

    using ElemPair = std::pair<size_t, std::shared_ptr<EBT>>;
    std::vector<ElemPair> elems;
    elems.reserve(bo->elements.size());
    for (auto &[idx, elem] : bo->elements) {
      elems.emplace_back(idx, elem);
    }

    std::sort(
        elems.begin(), elems.end(),
        [](const ElemPair &a, const ElemPair &b) { return a.first > b.first; });

    for (auto &[idx, elem] : elems) {
      bo->remove(op, idx);        
      bo->add(op, idx + 1, elem); 
    }

    std::shared_ptr<EBT> e = vobj::create<EBT>(SUPER::at(0));
    bo->add(op, 0, e);
    op.comps.push_back(
        std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));

    SPIN;
  }

  void push_front(T &&value, SLOC) {
    UPDATE;
    vobj::Operation &op = MODEL.addOp(sloc, "deque push_front");

    SUPER::push_front(std::move(value));

    using ElemPair = std::pair<size_t, std::shared_ptr<EBT>>;
    std::vector<ElemPair> elems;
    elems.reserve(bo->elements.size());
    for (auto &[idx, elem] : bo->elements) {
      elems.emplace_back(idx, elem);
    }
    std::sort(
        elems.begin(), elems.end(),
        [](const ElemPair &a, const ElemPair &b) { return a.first > b.first; });

    for (auto &[idx, elem] : elems) {
      bo->remove(op, idx);
      bo->add(op, idx + 1, elem);
    }

    std::shared_ptr<EBT> e = vobj::create<EBT>(SUPER::at(0));
    bo->add(op, 0, e);
    op.comps.push_back(
        std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));

    SPIN;
  }

  void pop_front(SLOC) {
    UPDATE;
    vobj::Operation &op = MODEL.addOp(sloc, "deque pop_front");

    if (!SUPER::empty()) {

      SUPER::pop_front();


      bo->remove(op, 0);

      using ElemPair = std::pair<size_t, std::shared_ptr<EBT>>;
      std::vector<ElemPair> elems;
      elems.reserve(bo->elements.size());
      for (auto &[idx, elem] : bo->elements) {
        if (idx == 0)
          continue;
        elems.emplace_back(idx, elem);
      }
      std::sort(elems.begin(), elems.end(),
                [](const ElemPair &a, const ElemPair &b) {
                  return a.first < b.first;
                });

      for (auto &[idx, elem] : elems) {
        bo->remove(op, idx);
        bo->add(op, idx - 1, elem);
      }
    }

    SPIN;
  }
};
} 
