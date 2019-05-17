//
// fill_2.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_2_0.txt or copy at http://www.boost.org/LICENSE_2_0.txt)
//

#ifndef FILL_2_HPP
#define FILL_2_HPP

#include "extension_property.hpp"

namespace algorithm {

//------------------------------------------------------------------------------
// fill #2: type-specific algorithm with no default

namespace extensions {

template <class ForwardIt>
struct fill_2_t : extension_property<fill_2_t<ForwardIt>> {};

template <class ForwardIt>
constexpr fill_2_t<ForwardIt> fill_2;

} // namespace extensions

template <class ForwardIt, class T>
void fill_2(ForwardIt first, ForwardIt last, const T& value)
{
  for (ForwardIt iter = first; iter != last; ++iter)
    *iter = value;
}

template <class Ex, class ForwardIt, class T>
void fill_2(const Ex& ex, ForwardIt first, ForwardIt last, const T& value)
{
  if constexpr (propria::can_require_concept_v<
      Ex, extensions::fill_2_t<ForwardIt>>)
  {
    propria::require_concept(ex,
        extensions::fill_2<ForwardIt>
      ).fill_2(first, last, value);
  }
  else
  {
    for (ForwardIt iter = first; iter != last; ++iter)
      ex.execute([&elem = *iter, value]{ elem = value; });
  }
}

//------------------------------------------------------------------------------

} // namespace algorithm

#endif // FILL_2_HPP
