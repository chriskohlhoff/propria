//
// global_allocator.hpp
// ~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef GLOBAL_ALLOCATOR_HPP
#define GLOBAL_ALLOCATOR_HPP

#include "allocator.hpp"

template <class T>
class global_allocator
{
public:
  global_allocator() = default;
  global_allocator(const global_allocator&) noexcept = default;

  T* allocate(std::size_t n) const
  {
    return static_cast<T*>(::operator new(n * sizeof(T)));
  }

  void deallocate(T* p) const
  {
    ::operator delete(p);
  }

  template <class U>
  global_allocator<U> require_concept(allocator_of<U>) const
  {
    return global_allocator<U>();
  }
};

static_assert(is_proto_allocator_v<global_allocator<char>>);
static_assert(is_allocator_of_v<global_allocator<char>, char>);

#endif // GLOBAL_ALLOCATOR_HPP
