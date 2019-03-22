//
// prefer.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_PREFER_HPP
#define PROPRIA_PREFER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "propria/detail/config.hpp"
#include "propria/detail/type_traits.hpp"
#include "propria/is_applicable_property.hpp"
#include "propria/traits/require_static.hpp"
#include "propria/traits/require_member.hpp"
#include "propria/traits/prefer_free.hpp"

namespace propria_prefer_fn {

using propria::detail::decay;
using propria::detail::declval;
using propria::detail::enable_if;
using propria::is_applicable_property;
using propria::traits::prefer_free;
using propria::traits::require_member;
using propria::traits::require_static;

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
      decay<Property>::type::is_preferable
      &&
      require_static<
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
      decay<Property>::type::is_preferable
      &&
      !require_static<
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
      decay<Property>::type::is_preferable
      &&
      !require_static<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      !require_member<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      prefer_free<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
    )
  >::type> :
  prefer_free<
    typename decay<T>::type,
    typename decay<Property>::type
  >
{
  PROPRIA_STATIC_CONSTEXPR(overload_type, overload = call_free);
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
      decay<Property>::type::is_preferable
      &&
      !require_static<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      !require_member<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      !prefer_free<
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
    PROPRIA_CONDITIONAL_NOEXCEPT((
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
    PROPRIA_CONDITIONAL_NOEXCEPT((
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
    PROPRIA_CONDITIONAL_NOEXCEPT((
      call_traits<T, void(Property)>::is_noexcept))
  {
    return prefer(
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
    PROPRIA_CONDITIONAL_NOEXCEPT((
      call_traits<T, void(P0, P1)>::is_noexcept))
  {
    return (*this)(
        (*this)(
          PROPRIA_MOVE_CAST(T)(t),
          PROPRIA_MOVE_CAST(P0)(p0)),
        PROPRIA_MOVE_CAST(P1)(p1));
  }

  template <typename T, typename P0, typename P1, typename PROPRIA_ELLIPSIS PN>
  PROPRIA_CONSTEXPR typename enable_if<
    call_traits<T, void(P0, P1, PN PROPRIA_ELLIPSIS)>::overload == n_props,
    typename call_traits<T, void(P0, P1, PN PROPRIA_ELLIPSIS)>::result_type
  >::type
  operator()(
      PROPRIA_MOVE_ARG(T) t,
      PROPRIA_MOVE_ARG(P0) p0,
      PROPRIA_MOVE_ARG(P1) p1,
      PROPRIA_MOVE_ARG(PN) PROPRIA_ELLIPSIS pn) const
    PROPRIA_CONDITIONAL_NOEXCEPT((
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

} // namespace propria_prefer_fn
namespace propria {
namespace {

static PROPRIA_CONSTEXPR const propria_prefer_fn::impl&
  prefer = propria_prefer_fn::static_instance<>::instance;

} // namespace

#if defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

template <typename T, typename... Properties>
struct can_prefer :
  detail::integral_constant<bool,
    propria_prefer_fn::call_traits<T, void(Properties...)>::overload
      != propria_prefer_fn::ill_formed>
{
};

#else // defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

template <typename T, typename P0 = void,
    typename P1 = void, typename P2 = void>
struct can_prefer :
  detail::integral_constant<bool,
    propria_prefer_fn::call_traits<T, void(P0, P1, P2)>::overload
      != propria_prefer_fn::ill_formed>
{
};

template <typename T, typename P0, typename P1>
struct can_prefer<T, P0, P1> :
  detail::integral_constant<bool,
    propria_prefer_fn::call_traits<T, void(P0, P1)>::overload
      != propria_prefer_fn::ill_formed>
{
};

template <typename T, typename P0>
struct can_prefer<T, P0> :
  detail::integral_constant<bool,
    propria_prefer_fn::call_traits<T, void(P0)>::overload
      != propria_prefer_fn::ill_formed>
{
};

template <typename T>
struct can_prefer<T> :
  detail::false_type
{
};

#endif // defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

#if defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename PROPRIA_ELLIPSIS Properties>
constexpr bool can_prefer_v
  = can_prefer<T, Properties PROPRIA_ELLIPSIS>::value;

#endif // defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace propria

#endif // PROPRIA_PREFER_HPP
