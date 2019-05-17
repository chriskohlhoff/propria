//
// executor_property.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXECUTOR_PROPERTY_HPP
#define EXECUTOR_PROPERTY_HPP

#include <propria.hpp>
#include <type_traits>
#include "execution.hpp"

namespace algorithm {

//------------------------------------------------------------------------------
// CRTP helper for extension properties.

namespace extensions {

template <typename Derived>
struct executor_property
{
  template <typename T>
  static constexpr bool is_applicable_property_v
    = propria::can_query_v<T, execution::executor_concept_t>;

  static constexpr bool is_requirable_concept = true;

  template <
    typename E,
    typename P = Derived,
    typename T = decltype(propria::query(std::declval<E>(), execution::executor_concept))
  >
  static constexpr bool static_query_v = std::is_convertible_v<T, P>;

  static constexpr bool value() { return true; }
};

} // namespace extensions

//------------------------------------------------------------------------------

} // namespace algorithm

#endif // EXECUTOR_PROPERTY_HPP
