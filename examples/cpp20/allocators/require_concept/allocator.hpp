//
// allocator.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <cstddef>

template <class T>
concept proto_allocator = requires(const T& t, std::size_t n)
{
  { T(t) } noexcept;
  { t.allocate(n) };
  { t.deallocate(nullptr) };
};

template <class T>
constexpr bool is_proto_allocator_v = proto_allocator<T>;

template <class T, class U>
concept typed_allocator = requires(const T& t, std::size_t n, U* p)
{
  { T(t) } noexcept;
  { static_cast<U*>(t.allocate(n)) };
  { t.deallocate(p) };
};

template <class T, class U>
constexpr bool is_allocator_of_v = typed_allocator<T, U>;

template <class T>
struct allocator_of
{
  template <class U>
  static constexpr bool is_applicable_property_v
    = is_proto_allocator_v<U>;

  static constexpr bool is_requirable_concept = true;

  template <class U>
  static constexpr bool static_query_v
    = is_allocator_of_v<U, T>;

  static constexpr bool value() { return true; }
};

#endif // ALLOCATOR_HPP
