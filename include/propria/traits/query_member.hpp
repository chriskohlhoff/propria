//
// traits/query_member.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_QUERY_MEMBER_HPP
#define PROPRIA_TRAITS_QUERY_MEMBER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "propria/detail/config.hpp"
#include "propria/detail/type_traits.hpp"

namespace propria {
namespace detail {

#if defined(PROPRIA_HAS_DECLTYPE) \
  && defined(PROPRIA_HAS_NOEXCEPT)

template <typename T, typename Property, typename = void>
struct query_member_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename Property>
struct query_member_trait<T, Property,
  typename void_type<
    decltype(declval<T>().query(declval<Property>()))
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    declval<T>().query(declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    declval<T>().query(declval<Property>())));
};

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)

template <typename T, typename Property>
struct query_member_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct query_member_default :
  detail::query_member_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct query_member :
  query_member_default<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_QUERY_MEMBER_HPP
