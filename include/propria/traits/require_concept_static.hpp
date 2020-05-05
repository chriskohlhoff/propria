//
// traits/require_concept_static.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_REQUIRE_CONCEPT_STATIC_HPP
#define PROPRIA_TRAITS_REQUIRE_CONCEPT_STATIC_HPP

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
struct require_concept_static_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
};

#if defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property>
struct require_concept_static_trait<T, Property,
  typename enable_if<
    Property::value() == Property::template static_query_v<T>
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);
};

#else // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

false_type require_concept_static_test(...);

template <typename T, typename Property>
true_type require_concept_static_test(T*, Property*,
    typename enable_if<
      Property::value() == Property::template static_query_v<T>
    >::type* = 0);

template <typename T, typename Property>
struct has_require_concept_static
{
  PROPRIA_STATIC_CONSTEXPR(bool, value =
    decltype((require_concept_static_test)(
      static_cast<T*>(0), static_cast<Property*>(0)))::value);
};

template <typename T, typename Property>
struct require_concept_static_trait<T, Property,
  typename enable_if<
    has_require_concept_static<T, Property>::value
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);
};

#endif // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)
      //   && defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
struct require_concept_static_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)
       //   && defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct require_concept_static :
  detail::require_concept_static_trait<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_REQUIRE_CONCEPT_STATIC_HPP
