//
// extension_property.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXTENSION_PROPERTY_HPP
#define EXTENSION_PROPERTY_HPP

#include <propria.hpp>
#include <type_traits>
#include "execution.hpp"

namespace algorithm {

//------------------------------------------------------------------------------
// CRTP helper for extension properties.

namespace extensions {

template <typename Derived>
struct extension_property
{
  template <typename T>
  static constexpr bool is_applicable_property_v
    = propria::can_query_v<T, execution::executor_concept_t>;

  static constexpr bool is_requirable_concept = true;

  template <
    typename E,
    typename P = Derived,
    typename = std::enable_if_t<
      (static_cast<void>(E::query(P{})), true),
      decltype(E::query(P{}))
    >
  >
  static constexpr bool static_query_v = E::query(P{});

  static constexpr bool value() { return true; }
};

} // namespace extensions

//------------------------------------------------------------------------------

} // namespace algorithm

#endif // EXTENSION_PROPERTY_HPP
