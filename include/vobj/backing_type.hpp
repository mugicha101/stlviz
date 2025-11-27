#pragma once

#include "vobj/primitive.hpp"
#include <type_traits>

#define IS_PRIMITIVE(T) std::is_same_v<typename vobj::BackingType<T>::type, vobj::Primitive<T>>
#define ASSERT_PRIMITIVE(T) static_assert(IS_PRIMITIVE(T), "T does not having a backing type of vobj::Primitive<T>");

// map vstd / primitive classes to backing vobj class
namespace vstd {
  template<typename T> struct vector;
  template<typename T, typename Container> struct stack;
  template<typename T> struct deque;
  template<typename T, typename Compare> struct set;
  template <std::size_t N> struct bitset;
  template<typename T> struct binary_tree;
  template<typename T> struct binary_search_tree;
  template<typename T> struct NumWrapper;
}

namespace vobj {
  template<typename T> struct List;
  template<typename T> struct Tree;
  
  template<typename T>
  struct BackingType {
    using type = vobj::Primitive<T>;
  };
  template<typename T>
  struct BackingType<vstd::vector<T>> {
    using type = vobj::List<std::shared_ptr<typename BackingType<T>::type>>;
  };

  template<typename T, typename Container>
  struct BackingType<vstd::stack<T, Container>> {
    using type = vobj::List<std::shared_ptr<typename BackingType<T>::type>>;
  };
  template <typename T>
  struct BackingType<vstd::deque<T>> {
    using type = vobj::List<std::shared_ptr<typename BackingType<T>::type>>;
  };

  template <typename T, typename Compare>
  struct BackingType<vstd::set<T, Compare>> {
    using type = vobj::List<std::shared_ptr<typename BackingType<T>::type>>;
  };

  template <std::size_t N> 
  struct BackingType<vstd::bitset<N>> {
    using type = vobj::List<std::shared_ptr<typename BackingType<bool>::type>>;
  };

  template<typename T>
  struct BackingType<vstd::binary_tree<T>> {
    using type = vobj::Tree<std::shared_ptr<typename BackingType<T>::type>>;
  };

  template<typename T>
  struct BackingType<vstd::binary_search_tree<T>> {
    using type = vobj::Tree<std::shared_ptr<typename BackingType<T>::type>>;
  };

  template<typename T>
  struct BackingType<vstd::NumWrapper<T>> {
    using type = typename BackingType<T>::type;
  };
}