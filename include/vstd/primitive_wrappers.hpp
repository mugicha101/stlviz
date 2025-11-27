#include "vstd/base.hpp"
#include "vobj/all.hpp"
#include <vector>
#include <iostream>
#include <memory>
#include <type_traits>

// reference: https://coliru.stacked-crooked.com/view?id=f5566f15c11c52e2db7189d602cc601a-f674c1a6d04c632b71a62362c0ccfc51
namespace vstd {
  // wrapper class for numeric primitive types
  template<typename T>
  class NumWrapper : public base {
  protected:
    ASSERT_PRIMITIVE(T);
    using CT = NumWrapper<T>;
    using BT = vobj::BackingType<T>::type;
    T value;
    std::shared_ptr<BT> bo = nullptr;

    bool _vstd_update_values(vobj::Operation &op) override {
      return bo->update(op, value);
    }
    static const char *name();

  public:

    operator T() const { return value; }
    NumWrapper(T v, SLOC) : value(v) {
      std::cout << "PRIMITIVE WRAPPER INIT AT " << sloc.line() << std::endl;
      OP(std::string(name()) + " initialization",
        bo = vobj::create<BT>(v);
        bo->o = (vstd::base *)this;
        std::cout << "BO UID " << bo->uid << " MAPPED TO PRIMITIVE WRAPPER " << name() << std::endl;
        std::cout << bo.get() << " " << this << std::endl;
        op.comps.push_back(std::make_unique<vobj::ConstructOp>(bo, sloc));
        op.comps.push_back(std::make_unique<vobj::AssignOp<T>>(bo, bo->value, v));
      )
    }
    NumWrapper() : NumWrapper(T{}, std::source_location::current()) {}
    NumWrapper(NumWrapper &nw) : NumWrapper(nw.value, std::source_location::current()) {}
    NumWrapper(const NumWrapper &nw) : NumWrapper(nw.value, std::source_location::current()) {}
    NumWrapper(NumWrapper &&nw) : bo(nw.bo), value(nw.value) {
      bo->o = this;
      nw.bo = nullptr; // prevent destructor from killing bo
    }
    ~NumWrapper() {
      SLOC;
      if (!bo) return;
      OP(std::string(name()) + " destruction",
        op.comps.push_back(std::make_unique<vobj::DestroyOp>(bo));
      )
    }

    NumWrapper &operator=(T v) { value = v; return *this; }
    NumWrapper &operator=(const NumWrapper &other) { value = other.value; return *this; }
    NumWrapper &operator+=(T v) { value += v; return *this; }
    NumWrapper &operator-=(T v) { value -= v; return *this; }
    NumWrapper &operator*=(T v) { value *= v; return *this; }

    NumWrapper operator+() const { return NumWrapper(+value); }
    NumWrapper operator-() const { return NumWrapper(-value); }
    NumWrapper operator!() const { return NumWrapper(!value); }

    template<typename U = T, typename = std::enable_if_t<std::is_integral<U>::value>>
    NumWrapper &operator++() { ++value; return *this; }

    template<typename U = T, typename = std::enable_if_t<std::is_integral<U>::value>>
    NumWrapper &operator--() { --value; return *this; }

    template<typename U = T, typename = std::enable_if_t<std::is_integral<U>::value>>
    NumWrapper operator++(int) { return NumWrapper(value++, NULL_SLOC); }

    template<typename U = T, typename = std::enable_if_t<std::is_integral<U>::value>>
    NumWrapper operator--(int) { return NumWrapper(value--, NULL_SLOC); }

    template<typename U = T, typename = std::enable_if_t<std::is_integral<U>::value>>
    NumWrapper &operator&=(T v) { value &= v; return *this; } 

    template<typename U = T, typename = std::enable_if_t<std::is_integral<U>::value>>
    NumWrapper &operator|=(T v) { value |= v; return *this; }

    template<typename U = T, typename = std::enable_if_t<std::is_integral<U>::value>>
    NumWrapper &operator^=(T v) { value ^= v; return *this; }

    template<typename U = T, typename = std::enable_if_t<std::is_integral<U>::value>>
    NumWrapper &operator<<=(T v) { value <<= v; return *this; }

    template<typename U = T, typename = std::enable_if_t<std::is_integral<U>::value>>
    NumWrapper &operator>>=(T v) { value >>= v; return *this; }

    template<typename U = T, typename = std::enable_if_t<std::is_integral<U>::value>>
    NumWrapper operator~() const { return NumWrapper(~value); }

    void _vstd_rename(std::string name, SLOC) override {
      OP("rename " + std::string(this->name()),
        op.comps.push_back(std::make_unique<vobj::RenameOp>(bo, bo->name, name));
      )
    }

    #undef UPD
  };

  #define WRAPPER(Name, WrapperType) \
    using Name = WrapperType; \
    template<> const char *WrapperType::name() { return #Name; }

  WRAPPER(Char, NumWrapper<char>)
  WRAPPER(Int, NumWrapper<int>)
  WRAPPER(Long, NumWrapper<long>)
  WRAPPER(LongLong, NumWrapper<long long>)
  WRAPPER(Unsigned, NumWrapper<unsigned>)
  WRAPPER(UnsignedLongLong, NumWrapper<unsigned long long>)
  WRAPPER(Float, NumWrapper<float>)
  WRAPPER(Double, NumWrapper<double>)
  WRAPPER(LongDouble, NumWrapper<long double>)
  #undef WRAPPER
};