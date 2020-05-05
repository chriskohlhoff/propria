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

class global_allocator
{
public:
  global_allocator() = default;
  global_allocator(const global_allocator&) noexcept = default;

  void* allocate(std::size_t n) const
  {
    return ::operator new(n);
  }

  void deallocate(void* p) const
  {
    ::operator delete(p);
  }
};

static_assert(is_allocator_v<global_allocator>);

#endif // GLOBAL_ALLOCATOR_HPP
