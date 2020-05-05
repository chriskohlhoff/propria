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
  auto possibly_aligned_allocator
    = propria::prefer(allocator, alignment{PAGE_SIZE});

  (void)possibly_aligned_allocator; // suppress unused variable warning

  std::cout << "single generic implementation\n";
  // more efficient when given page-aligned memory
}

#endif // ALGORITHM_HPP
