#pragma once

#include "vobj/all.hpp"
#include "vstd/base.hpp"
#include <algorithm>
#include <memory>
#include <set>
#include <vector>

namespace vstd {

template <typename T, typename Compare = std::less<T>>
class set : public std::set<T, Compare>, public base {
  using SUPER = std::set<T, Compare>;
  using iterator = typename SUPER::iterator;
  using const_iterator = typename SUPER::const_iterator;
  using EBT = typename vobj::BackingType<T>::type;

  ASSERT_PRIMITIVE(T);

  std::shared_ptr<vobj::List<EBT>> bo;

  bool _vstd_update_values(vobj::Operation &) override { return false; }

  void init_helper(std::source_location sloc) {
    OP("set initialization", bo = vobj::create<vobj::List<EBT>>();
       bo->o = (vstd::base *)this;
       op.comps.push_back(std::make_unique<vobj::ConstructOp>(bo, sloc));)
  }

  void rebuild(vobj::Operation &op) {
    std::vector<size_t> idxs;
    idxs.reserve(bo->elements.size());
    for (auto &p : bo->elements)
      idxs.push_back(p.first);
    std::sort(idxs.begin(), idxs.end(), std::greater<size_t>());
    for (size_t idx : idxs)
      bo->remove(op, idx);
    size_t i = 0;
    for (const T &x : *this) {
      auto e = vobj::create<EBT>(x);
      bo->add(op, i, e);
      op.comps.push_back(
          std::make_unique<vobj::AssignOp<T>>(e, e->value, e->latest));
      ++i;
    }
  }

public:
  void _vstd_rename(std::string name, SLOC) override {
    OP("rename set", op.comps.push_back(
                         std::make_unique<vobj::RenameOp>(bo, bo->name, name));)
  }

  set(SLOC) : SUPER() { init_helper(sloc); }

  set(const set &other, SLOC) : SUPER(other) { init_helper(sloc); }

  set(set &&other, SLOC) noexcept : SUPER(std::move(other)) {
    init_helper(sloc);
  }

  ~set() {
    SLOC;
    OP("set destruction", std::vector<size_t> idxs;
       idxs.reserve(bo->elements.size());
       for (auto &p : bo->elements) idxs.push_back(p.first);
       std::sort(idxs.begin(), idxs.end(), std::greater<size_t>());
       for (size_t idx : idxs) bo->remove(op, idx);
       op.comps.push_back(std::make_unique<vobj::DestroyOp>(bo));)
  }

  iterator begin() { return SUPER::begin(); }
  iterator end() { return SUPER::end(); }
  const_iterator begin() const { return SUPER::begin(); }
  const_iterator end() const { return SUPER::end(); }

  size_t size() const { return SUPER::size(); }
  bool empty() const { return SUPER::empty(); }

  std::pair<iterator, bool> insert(const T &value, SLOC) {
    std::pair<iterator, bool> res;
    OP("set insert", res = SUPER::insert(value); if (res.second) rebuild(op);)
    return res;
  }

  std::pair<iterator, bool> insert(T &&value, SLOC) {
    std::pair<iterator, bool> res;
    OP("set insert", res = SUPER::insert(std::move(value));
       if (res.second) rebuild(op);)
    return res;
  }

  size_t erase(const T &key, SLOC) {
    size_t count = 0;
    OP("set erase", count = SUPER::erase(key); if (count) rebuild(op);)
    return count;
  }
  void clear(SLOC) {
    OP("set clear",
      SUPER::clear();
      rebuild(op);
    )
  }
};

} // namespace vstd
