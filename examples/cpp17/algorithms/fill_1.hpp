//
// fill_1.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef FILL_1_HPP
#define FILL_1_HPP

#include "extension_property.hpp"

namespace algorithm {

//------------------------------------------------------------------------------
// fill #1: algorithm with no default

namespace extensions {

struct fill_1_t : extension_property<fill_1_t> {};

constexpr fill_1_t fill_1;

} // namespace extensions

//------------------------------------------------------------------------------

template <class ForwardIt, class T>
void fill_1(ForwardIt first, ForwardIt last, const T& value)
{
  for (ForwardIt iter = first; iter != last; ++iter)
    *iter = value;
}

template <class Ex, class ForwardIt, class T>
void fill_1(const Ex& ex, ForwardIt first, ForwardIt last, const T& value)
{
  if constexpr (propria::can_require_concept_v<Ex, extensions::fill_1_t>)
  {
    propria::require_concept(ex,
        extensions::fill_1
      ).fill_1(first, last, value);
  }
  else
  {
    for (ForwardIt iter = first; iter != last; ++iter)
      ex.execute([&elem = *iter, value]{ elem = value; });
  }
}

//------------------------------------------------------------------------------

} // namespace algorithm

#endif // FILL_1_HPP
