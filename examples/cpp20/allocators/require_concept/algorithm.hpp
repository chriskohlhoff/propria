//
// algorithm.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <propria.hpp>
#include <iostream>
#include "allocator.hpp"

template <class T>
struct my_class
{
};

template <proto_allocator Allocator>
void my_algorithm(/* ..., */ Allocator allocator)
{
  auto allocator_of_int
    = propria::require_concept(allocator, allocator_of<int>{});

  int* a = allocator_of_int.allocate(1);
  allocator_of_int.deallocate(a);

  auto allocator_of_my_class
    = propria::require_concept(allocator, allocator_of<my_class<int>>{});

  my_class<int>* b = allocator_of_my_class.allocate(1);
  allocator_of_my_class.deallocate(b);
}

#endif // ALGORITHM_HPP
