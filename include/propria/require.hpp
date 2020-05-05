//
// require.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_REQUIRE_HPP
#define PROPRIA_REQUIRE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "propria/detail/config.hpp"
#include "propria/detail/type_traits.hpp"
#include "propria/is_applicable_property.hpp"
#include "propria/traits/require_member.hpp"
#include "propria/traits/require_free.hpp"
#include "propria/traits/static_require.hpp"

namespace propria_require_fn {

using propria::detail::decay;
using propria::detail::declval;
using propria::detail::enable_if;
using propria::is_applicable_property;
using propria::traits::require_free;
using propria::traits::require_member;
using propria::traits::static_require;

enum overload_type
{
  identity,
  call_member,
  call_free,
  two_props,
  n_props,
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
      decay<Property>::type::is_requirable
      &&
      static_require<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
    )
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(overload_type, overload = identity);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = true);

#if defined(PROPRIA_HAS_MOVE)
  typedef PROPRIA_MOVE_ARG(T) result_type;
#else // defined(PROPRIA_HAS_MOVE)
  typedef PROPRIA_MOVE_ARG(typename decay<T>::type) result_type;
#endif // defined(PROPRIA_HAS_MOVE)
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
      decay<Property>::type::is_requirable
      &&
      !static_require<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      require_member<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
    )
  >::type> :
  require_member<
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
      decay<Property>::type::is_requirable
      &&
      !static_require<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      !require_member<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      require_free<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
    )
  >::type> :
  require_free<
    typename decay<T>::type,
    typename decay<Property>::type
  >
{
  PROPRIA_STATIC_CONSTEXPR(overload_type, overload = call_free);
};

template <typename T, typename P0, typename P1>
struct call_traits<T, void(P0, P1),
  typename enable_if<
    call_traits<T, void(P0)>::overload != ill_formed
    &&
    call_traits<
      typename call_traits<T, void(P0)>::result_type,
      void(P1)
    >::overload != ill_formed
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(overload_type, overload = two_props);

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept =
    (
      call_traits<T, void(P0)>::is_noexcept
      &&
      call_traits<
        typename call_traits<T, void(P0)>::result_type,
        void(P1)
      >::is_noexcept
    ));

  typedef typename call_traits<
    typename call_traits<T, void(P0)>::result_type,
    void(P1)
  >::result_type result_type;
};

template <typename T, typename P0, typename P1, typename PROPRIA_ELLIPSIS PN>
struct call_traits<T, void(P0, P1, PN PROPRIA_ELLIPSIS),
  typename enable_if<
    call_traits<T, void(P0)>::overload != ill_formed
    &&
    call_traits<
      typename call_traits<T, void(P0)>::result_type,
      void(P1, PN PROPRIA_ELLIPSIS)
    >::overload != ill_formed
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(overload_type, overload = n_props);

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept =
    (
      call_traits<T, void(P0)>::is_noexcept
      &&
      call_traits<
        typename call_traits<T, void(P0)>::result_type,
        void(P1, PN PROPRIA_ELLIPSIS)
      >::is_noexcept
    ));

  typedef typename call_traits<
    typename call_traits<T, void(P0)>::result_type,
    void(P1, PN PROPRIA_ELLIPSIS)
  >::result_type result_type;
};

struct impl
{
  template <typename T, typename Property>
  PROPRIA_CONSTEXPR typename enable_if<
    call_traits<T, void(Property)>::overload == identity,
    typename call_traits<T, void(Property)>::result_type
  >::type
  operator()(
      PROPRIA_MOVE_ARG(T) t,
      PROPRIA_MOVE_ARG(Property)) const
    PROPRIA_NOEXCEPT_IF((
      call_traits<T, void(Property)>::is_noexcept))
  {
    return PROPRIA_MOVE_CAST(T)(t);
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
    return PROPRIA_MOVE_CAST(T)(t).require(
        PROPRIA_MOVE_CAST(Property)(p));
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
    return require(
        PROPRIA_MOVE_CAST(T)(t),
        PROPRIA_MOVE_CAST(Property)(p));
  }

  template <typename T, typename P0, typename P1>
  PROPRIA_CONSTEXPR typename enable_if<
    call_traits<T, void(P0, P1)>::overload == two_props,
    typename call_traits<T, void(P0, P1)>::result_type
  >::type
  operator()(
      PROPRIA_MOVE_ARG(T) t,
      PROPRIA_MOVE_ARG(P0) p0,
      PROPRIA_MOVE_ARG(P1) p1) const
    PROPRIA_NOEXCEPT_IF((
      call_traits<T, void(P0, P1)>::is_noexcept))
  {
    return (*this)(
        (*this)(
          PROPRIA_MOVE_CAST(T)(t),
          PROPRIA_MOVE_CAST(P0)(p0)),
        PROPRIA_MOVE_CAST(P1)(p1));
  }

  template <typename T, typename P0, typename P1,
    typename PROPRIA_ELLIPSIS PN>
  PROPRIA_CONSTEXPR typename enable_if<
    call_traits<T, void(P0, P1, PN PROPRIA_ELLIPSIS)>::overload == n_props,
    typename call_traits<T, void(P0, P1, PN PROPRIA_ELLIPSIS)>::result_type
  >::type
  operator()(
      PROPRIA_MOVE_ARG(T) t,
      PROPRIA_MOVE_ARG(P0) p0,
      PROPRIA_MOVE_ARG(P1) p1,
      PROPRIA_MOVE_ARG(PN) PROPRIA_ELLIPSIS pn) const
    PROPRIA_NOEXCEPT_IF((
      call_traits<T, void(P0, P1, PN PROPRIA_ELLIPSIS)>::is_noexcept))
  {
    return (*this)(
        (*this)(
          PROPRIA_MOVE_CAST(T)(t),
          PROPRIA_MOVE_CAST(P0)(p0)),
        PROPRIA_MOVE_CAST(P1)(p1),
        PROPRIA_MOVE_CAST(PN)(pn) PROPRIA_ELLIPSIS);
  }
};

template <typename T = impl>
struct static_instance
{
  static const T instance;
};

template <typename T>
const T static_instance<T>::instance = {};

} // namespace propria_require_fn
namespace propria {
namespace {

static PROPRIA_CONSTEXPR const propria_require_fn::impl&
  require PROPRIA_UNUSED_VARIABLE
    = propria_require_fn::static_instance<>::instance;

} // namespace

#if defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

template <typename T, typename... Properties>
struct can_require :
  detail::integral_constant<bool,
    propria_require_fn::call_traits<T, void(Properties...)>::overload
      != propria_require_fn::ill_formed>
{
};

#else // defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

template <typename T, typename P0 = void,
    typename P1 = void, typename P2 = void>
struct can_require :
  detail::integral_constant<bool,
    propria_require_fn::call_traits<T, void(P0, P1, P2)>::overload
      != propria_require_fn::ill_formed>
{
};

template <typename T, typename P0, typename P1>
struct can_require<T, P0, P1> :
  detail::integral_constant<bool,
    propria_require_fn::call_traits<T, void(P0, P1)>::overload
      != propria_require_fn::ill_formed>
{
};

template <typename T, typename P0>
struct can_require<T, P0> :
  detail::integral_constant<bool,
    propria_require_fn::call_traits<T, void(P0)>::overload
      != propria_require_fn::ill_formed>
{
};

template <typename T>
struct can_require<T> :
  detail::false_type
{
};

#endif // defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

#if defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename PROPRIA_ELLIPSIS Properties>
constexpr bool can_require_v
  = can_require<T, Properties PROPRIA_ELLIPSIS>::value;

#endif // defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

#if defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

template <typename T, typename... Properties>
struct is_nothrow_require :
  detail::integral_constant<bool,
    propria_require_fn::call_traits<T, void(Properties...)>::is_noexcept>
{
};

#else // defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

template <typename T, typename P0 = void,
    typename P1 = void, typename P2 = void>
struct is_nothrow_require :
  detail::integral_constant<bool,
    propria_require_fn::call_traits<T, void(P0, P1, P2)>::is_noexcept>
{
};

template <typename T, typename P0, typename P1>
struct is_nothrow_require<T, P0, P1> :
  detail::integral_constant<bool,
    propria_require_fn::call_traits<T, void(P0, P1)>::is_noexcept>
{
};

template <typename T, typename P0>
struct is_nothrow_require<T, P0> :
  detail::integral_constant<bool,
    propria_require_fn::call_traits<T, void(P0)>::is_noexcept>
{
};

template <typename T>
struct is_nothrow_require<T> :
  detail::false_type
{
};

#endif // defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

#if defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename PROPRIA_ELLIPSIS Properties>
constexpr bool is_nothrow_require_v
  = is_nothrow_require<T, Properties PROPRIA_ELLIPSIS>::value;

#endif // defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace propria

#endif // PROPRIA_REQUIRE_HPP
