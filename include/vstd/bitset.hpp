#pragma once

#include "vobj/all.hpp"
#include "vstd/base.hpp"
#include <algorithm>
#include <bitset>
#include <cstddef>
#include <memory>
#include <vector>

namespace vstd {

template <std::size_t N> class bitset : public std::bitset<N>, public base {
  using SUPER = std::bitset<N>;
  using EBT = typename vobj::BackingType<bool>::type;

  ASSERT_PRIMITIVE(bool);

  std::shared_ptr<vobj::List<EBT>> bo;

  bool _vstd_update_values(vobj::Operation &op) override {
    bool changed = false;
    for (std::size_t i = 0; i < N; ++i) {
      auto it = bo->elements.find(i);
      if (it == bo->elements.end())
        continue;
      auto e = std::dynamic_pointer_cast<EBT>(it->second);
      changed = changed | e->update(op, SUPER::test(i));
    }
    return changed;
  }

  void init_helper(std::source_location sloc) {
    OP(
        "bitset initialization", bo = vobj::create<vobj::List<EBT>>();
        bo->o = (vstd::base *)this;
        op.comps.push_back(std::make_unique<vobj::ConstructOp>(bo, sloc));
        for (std::size_t i = 0; i < N; ++i) {
          auto e = vobj::create<EBT>(SUPER::test(i));
          bo->add(op, i, e);
          op.comps.push_back(
              std::make_unique<vobj::AssignOp<bool>>(e, e->value, e->latest));
        })
  }

public:
  void _vstd_rename(std::string name, SLOC) override {
    OP("rename bitset", op.comps.push_back(std::make_unique<vobj::RenameOp>(
                            bo, bo->name, name));)
  }

  std::string _vstd_type_name() const override {
    return "bitset";
  }

  bitset(SLOC) : SUPER() { init_helper(sloc); }
  bitset(const bitset &other, SLOC) : SUPER(other) { init_helper(sloc); }
  bitset(bitset &&other, SLOC) noexcept : SUPER(std::move(other)) {
    init_helper(sloc);
  }

  ~bitset() {
    SLOC;
    OP("bitset destruction", std::vector<std::size_t> idxs;
       idxs.reserve(bo->elements.size());
       for (auto &p : bo->elements) idxs.push_back(p.first);
       std::sort(idxs.begin(), idxs.end(), std::greater<std::size_t>());
       for (std::size_t idx : idxs) bo->remove(op, idx);
       op.comps.push_back(std::make_unique<vobj::DestroyOp>(bo));)
  }

  std::size_t size() const { return SUPER::size(); }

  bitset &set(std::size_t pos, bool value = true, SLOC) {
    SUPER::set(pos, value);
    UPDATE;
    SPIN;
    return *this;
  }

  bitset &reset(std::size_t pos, SLOC) {
    SUPER::reset(pos);
    UPDATE;
    SPIN;
    return *this;
  }

  bitset &flip(std::size_t pos, SLOC) {
    SUPER::flip(pos);
    UPDATE;
    SPIN;
    return *this;
  }

  bool test(std::size_t pos) const { return SUPER::test(pos); }
};

} // namespace vstd
