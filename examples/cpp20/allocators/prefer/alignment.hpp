//
// alignment.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ALIGNMENT_HPP
#define ALIGNMENT_HPP

#include <cstddef>
#include "allocator.hpp"

#ifndef PAGE_SIZE
# define PAGE_SIZE 4096
#endif

struct alignment
{
  template <class T>
  static constexpr bool is_applicable_property_v
    = is_allocator_v<T>;

  static constexpr bool is_requirable = true;
  static constexpr bool is_preferable = true;

  std::size_t requested_alignment_;
};

#endif // ALIGNMENT_HPP
