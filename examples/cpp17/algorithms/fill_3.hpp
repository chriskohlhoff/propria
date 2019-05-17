//
// fill_3.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_3_0.txt or copy at http://www.boost.org/LICENSE_3_0.txt)
//

#ifndef FILL_3_HPP
#define FILL_3_HPP

#include "extension_property.hpp"

namespace algorithm {

//------------------------------------------------------------------------------
// fill #3: algorithm no default

namespace extensions {

struct fill_3_t : extension_property<fill_3_t>
{
  template <class Ex>
  struct dflt
  {
    Ex ex;

    static constexpr bool query(fill_3_t)
    {
      return true;
    }

    template <class ForwardIt, class T>
    void fill_3(ForwardIt first, ForwardIt last, const T& value) const
    {
      for (ForwardIt iter = first; iter != last; ++iter)
        ex.execute([&elem = *iter, value]{ elem = value; });
    }
  };

  template <class Ex,
    std::enable_if_t<
      std::is_convertible_v<
        decltype(propria::query(std::declval<Ex>(), execution::executor_concept)),
        execution::oneway_t
      >
    >* = nullptr>
  friend auto require_concept(const Ex& ex, fill_3_t)
  {
    return dflt<Ex>{ex};
  }
};

constexpr fill_3_t fill_3;

} // namespace extensions

template <class ForwardIt, class T>
void fill_3(ForwardIt first, ForwardIt last, const T& value)
{
  for (ForwardIt iter = first; iter != last; ++iter)
    *iter = value;
}

template <class Ex, class ForwardIt, class T>
void fill_3(const Ex& ex, ForwardIt first, ForwardIt last, const T& value)
{
  propria::require_concept(ex,
      extensions::fill_3
    ).fill_3(first, last, value);
}

//------------------------------------------------------------------------------

} // namespace algorithm

#endif // FILL_3_HPP
