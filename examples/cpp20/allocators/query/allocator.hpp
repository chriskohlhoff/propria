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
concept allocator = requires(const T& t, std::size_t n, void* p)
{
  { T(t) } noexcept;
  { static_cast<void*>(t.allocate(n)) };
  { t.deallocate(p) };
};

template <class T>
constexpr bool is_allocator_v = allocator<T>;

#endif // ALLOCATOR_HPP
