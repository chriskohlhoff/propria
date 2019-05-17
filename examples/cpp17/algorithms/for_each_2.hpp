//
// for_each_2.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_2_0.txt or copy at http://www.boost.org/LICENSE_2_0.txt)
//

#ifndef FOR_EACH_2_HPP
#define FOR_EACH_2_HPP

#include "executor_property.hpp"

namespace algorithm {

//------------------------------------------------------------------------------
// for_each #2: type-specific algorithm with no default

namespace extensions {

template <class ForwardIt>
struct for_each_2_t : executor_property<for_each_2_t<ForwardIt>> {};

template <class ForwardIt>
constexpr for_each_2_t<ForwardIt> for_each_2;

} // namespace extensions

template <class InputIt, class UnaryFunction>
void for_each_2(InputIt first, InputIt last, UnaryFunction f)
{
  for (InputIt iter = first; iter != last; ++iter)
    f(*iter);
}

template <class Ex, class ForwardIt, class UnaryFunction2>
void for_each_2(const Ex& ex, ForwardIt first, ForwardIt last, UnaryFunction2 f)
{
  if constexpr (propria::can_require_concept_v<
      Ex, extensions::for_each_2_t<ForwardIt>>)
  {
    propria::require_concept(ex,
        extensions::for_each_2<ForwardIt>
      ).for_each_2(first, last, f);
  }
  else
  {
    for (ForwardIt iter = first; iter != last; ++iter)
      ex.execute([f, &elem = *iter]{ f(elem); });
  }
}

//------------------------------------------------------------------------------

} // namespace algorithm

#endif // FOR_EACH_2_HPP
