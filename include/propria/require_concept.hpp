//
// require_concept.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_REQUIRE_CONCEPT_HPP
#define PROPRIA_REQUIRE_CONCEPT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "propria/detail/config.hpp"
#include "propria/detail/type_traits.hpp"
#include "propria/is_applicable_property.hpp"
#include "propria/traits/require_concept_static.hpp"
#include "propria/traits/require_concept_member.hpp"
#include "propria/traits/require_concept_free.hpp"

namespace propria_require_concept_fn {

using propria::detail::decay;
using propria::detail::declval;
using propria::detail::enable_if;
using propria::is_applicable_property;
using propria::traits::require_concept_free;
using propria::traits::require_concept_member;
using propria::traits::require_concept_static;

enum overload_type
{
  identity,
  call_member,
  call_free,
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
      decay<Property>::type::is_requirable_concept
      &&
      require_concept_static<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
    )
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(overload_type, overload = identity);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = true);
  typedef PROPRIA_MOVE_ARG(T) result_type;
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
      decay<Property>::type::is_requirable_concept
      &&
      !require_concept_static<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      require_concept_member<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
    )
  >::type> :
  require_concept_member<
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
      decay<Property>::type::is_requirable_concept
      &&
      !require_concept_static<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      !require_concept_member<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
      &&
      require_concept_free<
        typename decay<T>::type,
        typename decay<Property>::type
      >::is_valid
    )
  >::type> :
  require_concept_free<
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
    return PROPRIA_MOVE_CAST(T)(t).require_concept(
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
    return require_concept(
        PROPRIA_MOVE_CAST(T)(t),
        PROPRIA_MOVE_CAST(Property)(p));
  }
};

template <typename T = impl>
struct static_instance
{
  static const T instance;
};

template <typename T>
const T static_instance<T>::instance = {};

} // namespace propria_require_concept_fn
namespace propria {
namespace {

static PROPRIA_CONSTEXPR const propria_require_concept_fn::impl&
  require_concept = propria_require_concept_fn::static_instance<>::instance;

} // namespace

template <typename T, typename Property>
struct can_require_concept :
  detail::integral_constant<bool,
    propria_require_concept_fn::call_traits<T, void(Property)>::overload !=
      propria_require_concept_fn::ill_formed>
{
};

#if defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
constexpr bool can_require_concept_v
  = can_require_concept<T, Property>::value;

#endif // defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace propria

#endif // PROPRIA_REQUIRE_CONCEPT_HPP
