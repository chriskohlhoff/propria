//
// traits/static_require.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_STATIC_REQUIRE_HPP
#define PROPRIA_TRAITS_STATIC_REQUIRE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "propria/detail/config.hpp"
#include "propria/detail/type_traits.hpp"

namespace propria {
namespace detail {

#if defined(PROPRIA_HAS_DECLTYPE) \
  && defined(PROPRIA_HAS_NOEXCEPT) \
  && defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property, typename = void>
struct static_require_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
};

#if defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property>
struct static_require_trait<T, Property,
  typename enable_if<
    Property::value() == Property::template static_query_v<T>
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);
};

#else // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

false_type static_require_test(...);

template <typename T, typename Property>
true_type static_require_test(T*, Property*,
    typename enable_if<
      Property::value() == Property::template static_query_v<T>
    >::type* = 0);

template <typename T, typename Property>
struct has_static_require
{
  PROPRIA_STATIC_CONSTEXPR(bool, value =
    decltype((static_require_test)(
      static_cast<T*>(0), static_cast<Property*>(0)))::value);
};

template <typename T, typename Property>
struct static_require_trait<T, Property,
  typename enable_if<
    has_static_require<T, Property>::value
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);
};

#endif // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)
      //   && defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
struct static_require_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)
       //   && defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct static_require_default : detail::static_require_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct static_require : static_require_default<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_STATIC_REQUIRE_HPP
