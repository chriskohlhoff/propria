//
// for_each_1.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef FOR_EACH_1_HPP
#define FOR_EACH_1_HPP

#include "executor_property.hpp"

namespace algorithm {

//------------------------------------------------------------------------------
// for_each #1: algorithm with no default

namespace extensions {

struct for_each_1_t : executor_property<for_each_1_t> {};

constexpr for_each_1_t for_each_1;

} // namespace extensions

//------------------------------------------------------------------------------

template <class InputIt, class UnaryFunction>
void for_each_1(InputIt first, InputIt last, UnaryFunction f)
{
  for (InputIt iter = first; iter != last; ++iter)
    f(*iter);
}

template <class Ex, class ForwardIt, class UnaryFunction2>
void for_each_1(const Ex& ex, ForwardIt first, ForwardIt last, UnaryFunction2 f)
{
  if constexpr (propria::can_require_concept_v<Ex, extensions::for_each_1_t>)
  {
    propria::require_concept(ex,
        extensions::for_each_1
      ).for_each_1(first, last, f);
  }
  else
  {
    for (ForwardIt iter = first; iter != last; ++iter)
      ex.execute([f, &elem = *iter]{ f(elem); });
  }
}

//------------------------------------------------------------------------------

} // namespace algorithm

#endif // FOR_EACH_1_HPP
