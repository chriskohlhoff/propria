//
// query.hpp
// ~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_QUERY_HPP
#define PROPRIA_QUERY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "propria/detail/config.hpp"
#include "propria/detail/type_traits.hpp"
#include "propria/is_applicable_property.hpp"
#include "propria/traits/query_member.hpp"
#include "propria/traits/query_free.hpp"
#include "propria/traits/static_query.hpp"

namespace propria_query_fn {

using propria::detail::decay;
using propria::detail::declval;
using propria::detail::enable_if;
using propria::is_applicable_property;
using propria::traits::query_free;
using propria::traits::query_member;
using propria::traits::static_query;

enum overload_type
{
  static_value,
  call_member,
  call_free,
  ill_formed
};

template <typename T, typename Properties, typename = void>
struct call_traits
{
  PROPRIA_STATIC_CONSTEXPR(overload_type, overload = ill_formed);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename Property>
struct call_traits<T, void(Property),
  typename enable_if<
    (
      is_applicable_property<
        typename decay<T>::type,
        typename decay<Property>::type
      >::value
      &&
      static_query<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
    )
  >::type> :
  static_query<
    typename decay<T>::type,
    typename decay<Property>::type
  >
{
  PROPRIA_STATIC_CONSTEXPR(overload_type, overload = static_value);
};

template <typename T, typename Property>
struct call_traits<T, void(Property),
  typename enable_if<
    (
      is_applicable_property<
        typename decay<T>::type,
        typename decay<Property>::type
      >::value
      &&
      !static_query<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      query_member<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
    )
  >::type> :
  query_member<
    typename decay<T>::type,
    typename decay<Property>::type
  >
{
  PROPRIA_STATIC_CONSTEXPR(overload_type, overload = call_member);
};

template <typename T, typename Property>
struct call_traits<T, void(Property),
  typename enable_if<
    (
      is_applicable_property<
        typename decay<T>::type,
        typename decay<Property>::type
      >::value
      &&
      !static_query<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      !query_member<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      query_free<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
    )
  >::type> :
  query_free<
    typename decay<T>::type,
    typename decay<Property>::type
  >
{
  PROPRIA_STATIC_CONSTEXPR(overload_type, overload = call_free);
};

struct impl
{
  template <typename T, typename Property>
  PROPRIA_CONSTEXPR typename enable_if<
    call_traits<T, void(Property)>::overload == static_value,
    typename call_traits<T, void(Property)>::result_type
  >::type
  operator()(
      PROPRIA_MOVE_ARG(T),
      PROPRIA_MOVE_ARG(Property)) const
    PROPRIA_NOEXCEPT_IF((
      call_traits<T, void(Property)>::is_noexcept))
  {
    return static_query<
      typename decay<T>::type,
      typename decay<Property>::type
    >::value();
  }

  template <typename T, typename Property>
  PROPRIA_CONSTEXPR typename enable_if<
    call_traits<T, void(Property)>::overload == call_member,
    typename call_traits<T, void(Property)>::result_type
  >::type
  operator()(
      PROPRIA_MOVE_ARG(T) t,
      PROPRIA_MOVE_ARG(Property) p) const
    PROPRIA_NOEXCEPT_IF((
      call_traits<T, void(Property)>::is_noexcept))
  {
    return PROPRIA_MOVE_CAST(T)(t).query(PROPRIA_MOVE_CAST(Property)(p));
  }

  template <typename T, typename Property>
  PROPRIA_CONSTEXPR typename enable_if<
    call_traits<T, void(Property)>::overload == call_free,
    typename call_traits<T, void(Property)>::result_type
  >::type
  operator()(
      PROPRIA_MOVE_ARG(T) t,
      PROPRIA_MOVE_ARG(Property) p) const
    PROPRIA_NOEXCEPT_IF((
      call_traits<T, void(Property)>::is_noexcept))
  {
    return query(PROPRIA_MOVE_CAST(T)(t), PROPRIA_MOVE_CAST(Property)(p));
  }
};

template <typename T = impl>
struct static_instance
{
  static const T instance;
};

template <typename T>
const T static_instance<T>::instance = {};

} // namespace propria_query_fn
namespace propria {
namespace {

static PROPRIA_CONSTEXPR const propria_query_fn::impl&
  query = propria_query_fn::static_instance<>::instance;

} // namespace

template <typename T, typename Property>
struct can_query :
  detail::integral_constant<bool,
    propria_query_fn::call_traits<T, void(Property)>::overload !=
      propria_query_fn::ill_formed>
{
};

#if defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
constexpr bool can_query_v
  = can_query<T, Property>::value;

#endif // defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
struct is_nothrow_query :
  detail::integral_constant<bool,
    propria_query_fn::call_traits<T, void(Property)>::is_noexcept>
{
};

} // namespace propria

#endif // PROPRIA_QUERY_HPP
