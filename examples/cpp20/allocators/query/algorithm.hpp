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
#include "alignment.hpp"

template <allocator Allocator>
void my_algorithm(/* ..., */ Allocator allocator)
{
  (void)allocator; // suppress unused parameter warning

  if constexpr (propria::can_query_v<Allocator, alignment>)
  {
    if (propria::query(allocator, alignment{}) == PAGE_SIZE)
    {
      std::cout << "special optimised implementation\n";
      return;
    }
  }

  std::cout << "generic fallback implementation\n";
}

#endif // ALGORITHM_HPP
