//
// for_each_3.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_3_0.txt or copy at http://www.boost.org/LICENSE_3_0.txt)
//

#ifndef FOR_EACH_3_HPP
#define FOR_EACH_3_HPP

#include "executor_property.hpp"

namespace algorithm {

//------------------------------------------------------------------------------
// for_each #2: type-specific algorithm with no default

namespace extensions {

struct for_each_3_t : executor_property<for_each_3_t>
{
  template <class Ex>
  struct dflt
  {
    Ex ex;

    static constexpr for_each_3_t query(execution::executor_concept_t)
    {
      return {};
    }

    template <class ForwardIt, class UnaryFunction2>
    void for_each_3(ForwardIt first, ForwardIt last, UnaryFunction2 f) const
    {
      for (ForwardIt iter = first; iter != last; ++iter)
        ex.execute([f, &elem = *iter]{ f(elem); });
    }
  };

  template <class Ex,
    std::enable_if_t<
      std::is_convertible_v<
        decltype(propria::query(std::declval<Ex>(), execution::executor_concept)),
        execution::oneway_t
      >
    >* = nullptr>
  friend auto require_concept(const Ex& ex, for_each_3_t)
  {
    return dflt<Ex>{ex};
  }
};

constexpr for_each_3_t for_each_3;

} // namespace extensions

template <class InputIt, class UnaryFunction>
void for_each_3(InputIt first, InputIt last, UnaryFunction f)
{
  for (InputIt iter = first; iter != last; ++iter)
    f(*iter);
}

template <class Ex, class ForwardIt, class UnaryFunction2>
void for_each_3(const Ex& ex, ForwardIt first, ForwardIt last, UnaryFunction2 f)
{
  propria::require_concept(ex,
      extensions::for_each_3
    ).for_each_3(first, last, f);
}

//------------------------------------------------------------------------------

} // namespace algorithm

#endif // FOR_EACH_3_HPP
