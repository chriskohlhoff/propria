//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "allocator.hpp"
#include "algorithm.hpp"
#include "global_allocator.hpp"
#include "monotonic_allocator.hpp"

int main()
{
  my_algorithm(/* ..., */ global_allocator{});

  monotonic_resource resource1(8192);
  my_algorithm(/* ..., */ resource1.get_allocator());

  monotonic_resource resource2(8192);
  my_algorithm(/* ..., */ monotonic_allocator(resource2, PAGE_SIZE));
}
