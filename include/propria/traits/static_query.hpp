//
// traits/static_query.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_STATIC_QUERY_HPP
#define PROPRIA_TRAITS_STATIC_QUERY_HPP

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
struct static_query_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
};

template <typename T, typename Property>
struct static_query_trait<T, Property,
  typename void_type<
    decltype(Property::template static_query_v<T>)
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(Property::template static_query_v<T>);

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept =
    noexcept(Property::template static_query_v<T>));

  static PROPRIA_CONSTEXPR result_type value() noexcept(is_noexcept)
  {
    return Property::template static_query_v<T>;
  }
};

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)
      //   && defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
struct static_query_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)
       //   && defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct static_query_default : detail::static_query_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct static_query : static_query_default<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_STATIC_QUERY_HPP
