//
// propria.hpp
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_HPP
#define PROPRIA_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

//
// is_applicable_property.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_IS_APPLICABLE_PROPERTY_HPP
#define PROPRIA_IS_APPLICABLE_PROPERTY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

//
// detail/config.hpp
// ~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_DETAIL_CONFIG_HPP
#define PROPRIA_DETAIL_CONFIG_HPP

// Microsoft Visual C++ detection.
#if !defined(PROPRIA_MSVC)
# if defined(_MSC_VER) && (defined(__INTELLISENSE__) \
      || (!defined(__MWERKS__) && !defined(__EDG_VERSION__)))
#  define PROPRIA_MSVC _MSC_VER
# endif // defined(_MSC_VER) && (defined(__INTELLISENSE__)
        // || (!defined(__MWERKS__) && !defined(__EDG_VERSION__)))
#endif // !defined(PROPRIA_MSVC)

// Clang / libc++ detection.
#if defined(__clang__)
# if (__cplusplus >= 201103)
#  if __has_include(<__config>)
#   include <__config>
#   if defined(_LIBCPP_VERSION)
#    define PROPRIA_HAS_CLANG_LIBCXX 1
#   endif // defined(_LIBCPP_VERSION)
#  endif // __has_include(<__config>)
# endif // (__cplusplus >= 201103)
#endif // defined(__clang__)

// Support move construction and assignment on compilers known to allow it.
#if !defined(PROPRIA_HAS_MOVE)
# if !defined(PROPRIA_DISABLE_MOVE)
#  if defined(__clang__)
#   if __has_feature(__cxx_rvalue_references__)
#    define PROPRIA_HAS_MOVE 1
#   endif // __has_feature(__cxx_rvalue_references__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define PROPRIA_HAS_MOVE 1
#    endif // (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(PROPRIA_MSVC)
#   if (_MSC_VER >= 1700)
#    define PROPRIA_HAS_MOVE 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(PROPRIA_MSVC)
#  if defined(__INTEL_CXX11_MODE__)
#    if defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1500)
#      define PROPRIA_HAS_MOVE 1
#    endif // defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 1500)
#    if defined(__ICL) && (__ICL >= 1500)
#      define PROPRIA_HAS_MOVE 1
#    endif // defined(__ICL) && (__ICL >= 1500)
#  endif // defined(__INTEL_CXX11_MODE__)
# endif // !defined(PROPRIA_DISABLE_MOVE)
#endif // !defined(PROPRIA_HAS_MOVE)

// If PROPRIA_MOVE_CAST isn't defined, and move support is available, define
// PROPRIA_MOVE_ARG and PROPRIA_MOVE_CAST to take advantage of rvalue
// references and perfect forwarding.
#if defined(PROPRIA_HAS_MOVE) && !defined(PROPRIA_MOVE_CAST)
# define PROPRIA_MOVE_ARG(type) type&&
# define PROPRIA_MOVE_ARG2(type1, type2) type1, type2&&
# define PROPRIA_MOVE_CAST(type) static_cast<type&&>
# define PROPRIA_MOVE_CAST2(type1, type2) static_cast<type1, type2&&>
#endif // defined(PROPRIA_HAS_MOVE) && !defined(PROPRIA_MOVE_CAST)

// If PROPRIA_MOVE_CAST still isn't defined, default to a C++03-compatible
// implementation. Note that older g++ and MSVC versions don't like it when you
// pass a non-member function through a const reference, so for most compilers
// we'll play it safe and stick with the old approach of passing the handler by
// value.
#if !defined(PROPRIA_MOVE_CAST)
# if defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
#   define PROPRIA_MOVE_ARG(type) const type&
#  else // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
#   define PROPRIA_MOVE_ARG(type) type
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 1)) || (__GNUC__ > 4)
# elif defined(PROPRIA_MSVC)
#  if (_MSC_VER >= 1400)
#   define PROPRIA_MOVE_ARG(type) const type&
#  else // (_MSC_VER >= 1400)
#   define PROPRIA_MOVE_ARG(type) type
#  endif // (_MSC_VER >= 1400)
# else
#  define PROPRIA_MOVE_ARG(type) type
# endif
# define PROPRIA_MOVE_CAST(type) static_cast<const type&>
# define PROPRIA_MOVE_CAST2(type1, type2) static_cast<const type1, type2&>
#endif // !defined(PROPRIA_MOVE_CAST)

// Support variadic templates on compilers known to allow it.
#if !defined(PROPRIA_HAS_VARIADIC_TEMPLATES)
# if !defined(PROPRIA_DISABLE_VARIADIC_TEMPLATES)
#  if defined(__clang__)
#   if __has_feature(__cxx_variadic_templates__)
#    define PROPRIA_HAS_VARIADIC_TEMPLATES 1
#   endif // __has_feature(__cxx_variadic_templates__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#    if (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define PROPRIA_HAS_VARIADIC_TEMPLATES 1
#    endif // (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(PROPRIA_MSVC)
#   if (_MSC_VER >= 1900)
#    define PROPRIA_HAS_VARIADIC_TEMPLATES 1
#   endif // (_MSC_VER >= 1900)
#  endif // defined(PROPRIA_MSVC)
# endif // !defined(PROPRIA_DISABLE_VARIADIC_TEMPLATES)
#endif // !defined(PROPRIA_HAS_VARIADIC_TEMPLATES)
#if !defined(PROPRIA_ELLIPSIS)
# if defined(PROPRIA_HAS_VARIADIC_TEMPLATES)
#  define PROPRIA_ELLIPSIS ...
# else // defined(PROPRIA_HAS_VARIADIC_TEMPLATES)
#  define PROPRIA_ELLIPSIS
# endif // defined(PROPRIA_HAS_VARIADIC_TEMPLATES)
#endif // !defined(PROPRIA_ELLIPSIS)

// Support deleted functions on compilers known to allow it.
#if !defined(PROPRIA_DELETED)
# if defined(__GNUC__)
#  if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#   if (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define PROPRIA_DELETED = delete
#   endif // (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#  endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
# endif // defined(__GNUC__)
# if defined(__clang__)
#  if __has_feature(__cxx_deleted_functions__)
#   define PROPRIA_DELETED = delete
#  endif // __has_feature(__cxx_deleted_functions__)
# endif // defined(__clang__)
# if defined(PROPRIA_MSVC)
#  if (_MSC_VER >= 1900)
#   define PROPRIA_DELETED = delete
#  endif // (_MSC_VER >= 1900)
# endif // defined(PROPRIA_MSVC)
# if !defined(PROPRIA_DELETED)
#  define PROPRIA_DELETED
# endif // !defined(PROPRIA_DELETED)
#endif // !defined(PROPRIA_DELETED)

// Support constexpr on compilers known to allow it.
#if !defined(PROPRIA_HAS_CONSTEXPR)
# if !defined(PROPRIA_DISABLE_CONSTEXPR)
#  if defined(__clang__)
#   if __has_feature(__cxx_constexpr__)
#    define PROPRIA_HAS_CONSTEXPR 1
#   endif // __has_feature(__cxx_constexr__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define PROPRIA_HAS_CONSTEXPR 1
#    endif // (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(PROPRIA_MSVC)
#   if (_MSC_VER >= 1900)
#    define PROPRIA_HAS_CONSTEXPR 1
#   endif // (_MSC_VER >= 1900)
#  endif // defined(PROPRIA_MSVC)
# endif // !defined(PROPRIA_DISABLE_CONSTEXPR)
#endif // !defined(PROPRIA_HAS_CONSTEXPR)
#if !defined(PROPRIA_CONSTEXPR)
# if defined(PROPRIA_HAS_CONSTEXPR)
#  define PROPRIA_CONSTEXPR constexpr
# else // defined(PROPRIA_HAS_CONSTEXPR)
#  define PROPRIA_CONSTEXPR
# endif // defined(PROPRIA_HAS_CONSTEXPR)
#endif // !defined(PROPRIA_CONSTEXPR)
#if !defined(PROPRIA_STATIC_CONSTEXPR)
# if defined(PROPRIA_HAS_CONSTEXPR)
#  define PROPRIA_STATIC_CONSTEXPR(type, assignment) \
    static constexpr type assignment
# else // defined(PROPRIA_HAS_CONSTEXPR)
#  define PROPRIA_STATIC_CONSTEXPR(type, assignment) \
    static const type assignment
# endif // defined(PROPRIA_HAS_CONSTEXPR)
#endif // !defined(PROPRIA_STATIC_CONSTEXPR)
#if !defined(PROPRIA_STATIC_CONSTEXPR_DEFAULT_INIT)
# if defined(PROPRIA_HAS_CONSTEXPR)
#  define PROPRIA_STATIC_CONSTEXPR_DEFAULT_INIT(type, name) \
    static constexpr const type name{}
# else // defined(PROPRIA_HAS_CONSTEXPR)
#  define PROPRIA_STATIC_CONSTEXPR_DEFAULT_INIT(type, name) \
    static const type name
# endif // defined(PROPRIA_HAS_CONSTEXPR)
#endif // !defined(PROPRIA_STATIC_CONSTEXPR_DEFAULT_INIT)

// Support noexcept on compilers known to allow it.
#if !defined(PROPRIA_HAS_NOEXCEPT)
# if !defined(PROPRIA_DISABLE_NOEXCEPT)
#  if defined(__clang__)
#   if __has_feature(__cxx_noexcept__)
#    define PROPRIA_HAS_NOEXCEPT 1
#   endif // __has_feature(__cxx_noexcept__)
#  elif defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#      define PROPRIA_HAS_NOEXCEPT 1
#    endif // (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  elif defined(PROPRIA_MSVC)
#   if (_MSC_VER >= 1900)
#    define PROPRIA_HAS_NOEXCEPT 1
#   endif // (_MSC_VER >= 1900)
#  endif // defined(PROPRIA_MSVC)
# endif // !defined(PROPRIA_DISABLE_NOEXCEPT)
# if !defined(PROPRIA_NOEXCEPT)
# endif // !defined(PROPRIA_NOEXCEPT)
# if !defined(PROPRIA_NOEXCEPT_OR_NOTHROW)
# endif // !defined(PROPRIA_NOEXCEPT_OR_NOTHROW)
#endif // !defined(PROPRIA_HAS_NOEXCEPT)
#if !defined(PROPRIA_NOEXCEPT)
# if defined(PROPRIA_HAS_NOEXCEPT)
#  define PROPRIA_NOEXCEPT noexcept(true)
# else // defined(PROPRIA_HAS_NOEXCEPT)
#  define PROPRIA_NOEXCEPT
# endif // defined(PROPRIA_HAS_NOEXCEPT)
#endif // !defined(PROPRIA_NOEXCEPT)
#if !defined(PROPRIA_NOEXCEPT_OR_NOTHROW)
# if defined(PROPRIA_HAS_NOEXCEPT)
#  define PROPRIA_NOEXCEPT_OR_NOTHROW noexcept(true)
# else // defined(PROPRIA_HAS_NOEXCEPT)
#  define PROPRIA_NOEXCEPT_OR_NOTHROW throw()
# endif // defined(PROPRIA_HAS_NOEXCEPT)
#endif // !defined(PROPRIA_NOEXCEPT_OR_NOTHROW)
#if !defined(PROPRIA_NOEXCEPT_IF)
# if defined(PROPRIA_HAS_NOEXCEPT)
#  define PROPRIA_NOEXCEPT_IF(c) noexcept(c)
# else // defined(PROPRIA_HAS_NOEXCEPT)
#  define PROPRIA_NOEXCEPT_IF(c)
# endif // defined(PROPRIA_HAS_NOEXCEPT)
#endif // !defined(PROPRIA_NOEXCEPT_IF)

// Support automatic type deduction on compilers known to support it.
#if !defined(PROPRIA_HAS_DECLTYPE)
# if !defined(PROPRIA_DISABLE_DECLTYPE)
#  if defined(__clang__)
#   if __has_feature(__cxx_decltype__)
#    define PROPRIA_HAS_DECLTYPE 1
#   endif // __has_feature(__cxx_decltype__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#    if (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define PROPRIA_HAS_DECLTYPE 1
#    endif // (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(PROPRIA_MSVC)
#   if (_MSC_VER >= 1800)
#    define PROPRIA_HAS_DECLTYPE 1
#   endif // (_MSC_VER >= 1800)
#  endif // defined(PROPRIA_MSVC)
# endif // !defined(PROPRIA_DISABLE_DECLTYPE)
#endif // !defined(PROPRIA_HAS_DECLTYPE)

// Support alias templates on compilers known to allow it.
#if !defined(PROPRIA_HAS_ALIAS_TEMPLATES)
# if !defined(PROPRIA_DISABLE_ALIAS_TEMPLATES)
#  if defined(__clang__)
#   if __has_feature(__cxx_alias_templates__)
#    define PROPRIA_HAS_ALIAS_TEMPLATES 1
#   endif // __has_feature(__cxx_alias_templates__)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#    if (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define PROPRIA_HAS_ALIAS_TEMPLATES 1
#    endif // (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(PROPRIA_MSVC)
#   if (_MSC_VER >= 1900)
#    define PROPRIA_HAS_ALIAS_TEMPLATES 1
#   endif // (_MSC_VER >= 1900)
#  endif // defined(PROPRIA_MSVC)
# endif // !defined(PROPRIA_DISABLE_ALIAS_TEMPLATES)
#endif // !defined(PROPRIA_HAS_ALIAS_TEMPLATES)

// Support template variables on compilers known to allow it.
#if !defined(PROPRIA_HAS_VARIABLE_TEMPLATES)
# if !defined(PROPRIA_DISABLE_VARIABLE_TEMPLATES)
#  if defined(__clang__)
#   if (__cplusplus >= 201402)
#    if __has_feature(__cxx_variable_templates__)
#     define PROPRIA_HAS_VARIABLE_TEMPLATES 1
#    endif // __has_feature(__cxx_variable_templates__)
#   endif // (__cplusplus >= 201402)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if (__GNUC__ >= 5)
#    if (__cplusplus >= 201402)
#     define PROPRIA_HAS_VARIABLE_TEMPLATES 1
#    endif // (__cplusplus >= 201402)
#   endif // (__GNUC__ >= 5)
#  endif // defined(__GNUC__)
#  if defined(PROPRIA_MSVC)
#   if (_MSC_VER >= 1901)
#    define PROPRIA_HAS_VARIABLE_TEMPLATES 1
#   endif // (_MSC_VER >= 1901)
#  endif // defined(PROPRIA_MSVC)
# endif // !defined(PROPRIA_DISABLE_VARIABLE_TEMPLATES)
#endif // !defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

// Enable workarounds for lack of working expression SFINAE.
#if !defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)
# if !defined(PROPRIA_DISABLE_WORKING_EXPRESSION_SFINAE)
#  if !defined(PROPRIA_MSVC)
#   if (__cplusplus >= 201103)
#    define PROPRIA_HAS_WORKING_EXPRESSION_SFINAE 1
#   endif // (__cplusplus >= 201103)
#  endif // defined(PROPRIA_MSVC)
# endif // !defined(PROPRIA_DISABLE_WORKING_EXPRESSION_SFINAE)
#endif // !defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

// Standard library support for type traits.
#if !defined(PROPRIA_HAS_STD_TYPE_TRAITS)
# if !defined(PROPRIA_DISABLE_STD_TYPE_TRAITS)
#  if defined(__clang__)
#   if defined(PROPRIA_HAS_CLANG_LIBCXX)
#    define PROPRIA_HAS_STD_TYPE_TRAITS 1
#   elif (__cplusplus >= 201103)
#    if __has_include(<type_traits>)
#     define PROPRIA_HAS_STD_TYPE_TRAITS 1
#    endif // __has_include(<type_traits>)
#   endif // (__cplusplus >= 201103)
#  endif // defined(__clang__)
#  if defined(__GNUC__)
#   if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#    if (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#     define PROPRIA_HAS_STD_TYPE_TRAITS 1
#    endif // (__cplusplus >= 201103) || defined(__GXX_EXPERIMENTAL_CXX0X__)
#   endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  endif // defined(__GNUC__)
#  if defined(PROPRIA_MSVC)
#   if (_MSC_VER >= 1700)
#    define PROPRIA_HAS_STD_TYPE_TRAITS 1
#   endif // (_MSC_VER >= 1700)
#  endif // defined(PROPRIA_MSVC)
# endif // !defined(PROPRIA_DISABLE_STD_TYPE_TRAITS)
#endif // !defined(PROPRIA_HAS_STD_TYPE_TRAITS)

// Some versions of gcc generate spurious warnings about unused variables.
#if defined(__GNUC__)
# if (__GNUC__ >= 4)
#  define PROPRIA_UNUSED_VARIABLE __attribute__((__unused__))
# endif // (__GNUC__ >= 4)
#endif // defined(__GNUC__)
#if !defined(PROPRIA_UNUSED_VARIABLE)
# define PROPRIA_UNUSED_VARIABLE
#endif // !defined(ASIO_UNUSED_VARIABLE)

#endif // PROPRIA_DETAIL_CONFIG_HPP

//
// detail/type_traits.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPL_DETAIL_TYPE_TRAITS_HPP
#define PROPL_DETAIL_TYPE_TRAITS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#if defined(PROPRIA_HAS_STD_TYPE_TRAITS)
# include <type_traits>
#else // defined(PROPRIA_HAS_STD_TYPE_TRAITS)
# include <boost/type_traits/decay.hpp>
# include <boost/utility/declval.hpp>
# include <boost/utility/enable_if.hpp>
#endif // defined(PROPRIA_HAS_STD_TYPE_TRAITS)

namespace propria {
namespace detail {

#if defined(PROPRIA_HAS_STD_TYPE_TRAITS)

using std::decay;
using std::declval;
using std::enable_if;
using std::false_type;
using std::integral_constant;
using std::true_type;

#else // defined(PROPRIA_HAS_STD_TYPE_TRAITS)

using boost::decay;
using boost::declval;
template <bool Condition, typename Type = void>
struct enable_if : boost::enable_if_c<Condition, Type> {};
using boost::false_type;
using boost::integral_constant;
using boost::true_type;

#endif // defined(PROPRIA_HAS_STD_TYPE_TRAITS)

template <typename> struct void_type { typedef void type; };

} // namespace detail
} // namespace propria

#endif // PROPL_DETAIL_TYPE_TRAITS_HPP

namespace propria {
namespace detail {

template <typename T, typename Property, typename = void>
struct is_applicable_property_trait : false_type
{
};

#if defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
struct is_applicable_property_trait<T, Property,
  typename void_type<
    typename enable_if<
      !!Property::template is_applicable_property_v<T>
    >::type
  >::type> : true_type
{
};

#endif // defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace detail

template <typename T, typename Property, typename = void>
struct is_applicable_property :
  detail::is_applicable_property_trait<T, Property>
{
};

#if defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
PROPRIA_CONSTEXPR const bool is_applicable_property_v
  = is_applicable_property<T, Property>::value;

#endif // defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace propria

#endif // PROPRIA_IS_APPLICABLE_PROPERTY_HPP

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

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

//
// traits/require_concept_member.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_REQUIRE_CONCEPT_MEMBER_HPP
#define PROPRIA_TRAITS_REQUIRE_CONCEPT_MEMBER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace propria {
namespace detail {

#if defined(PROPRIA_HAS_DECLTYPE) \
  && defined(PROPRIA_HAS_NOEXCEPT)

template <typename T, typename Property, typename = void>
struct require_concept_member_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename Property>
struct require_concept_member_trait<T, Property,
  typename void_type<
    decltype(declval<T>().require_concept(declval<Property>()))
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    declval<T>().require_concept(declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    declval<T>().require_concept(declval<Property>())));
};

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)

template <typename T, typename Property>
struct require_concept_member_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct require_concept_member_default :
  detail::require_concept_member_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct require_concept_member :
  require_concept_member_default<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_REQUIRE_CONCEPT_MEMBER_HPP

//
// traits/require_concept_free.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_REQUIRE_CONCEPT_FREE_HPP
#define PROPRIA_TRAITS_REQUIRE_CONCEPT_FREE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace propria {
namespace detail {

#if defined(PROPRIA_HAS_DECLTYPE) \
  && defined(PROPRIA_HAS_NOEXCEPT)

#if defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property, typename = void>
struct require_concept_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename Property>
struct require_concept_free_trait<T, Property,
  typename void_type<
    decltype(require_concept(declval<T>(), declval<Property>()))
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    require_concept(declval<T>(), declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    require_concept(declval<T>(), declval<Property>())));
};

#else // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property,
  typename = decltype(require_concept(declval<T>(), declval<Property>()))>
struct require_concept_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    require_concept(declval<T>(), declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    require_concept(declval<T>(), declval<Property>())));
};

struct no_require_concept {};

template <typename T, typename Property>
no_require_concept require_concept(T, Property);

template <typename T, typename Property>
struct require_concept_free_trait<T, Property, no_require_concept>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)

template <typename T, typename Property>
struct require_concept_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct require_concept_free_default :
  detail::require_concept_free_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct require_concept_free :
  require_concept_free_default<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_REQUIRE_CONCEPT_FREE_HPP

//
// traits/static_require_concept.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_STATIC_REQUIRE_CONCEPT_HPP
#define PROPRIA_TRAITS_STATIC_REQUIRE_CONCEPT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace propria {
namespace detail {

#if defined(PROPRIA_HAS_DECLTYPE) \
  && defined(PROPRIA_HAS_NOEXCEPT) \
  && defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property, typename = void>
struct static_require_concept_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
};

#if defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property>
struct static_require_concept_trait<T, Property,
  typename enable_if<
    Property::value() == Property::template static_query_v<T>
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);
};

#else // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

false_type static_require_concept_test(...);

template <typename T, typename Property>
true_type static_require_concept_test(T*, Property*,
    typename enable_if<
      Property::value() == Property::template static_query_v<T>
    >::type* = 0);

template <typename T, typename Property>
struct has_static_require_concept
{
  PROPRIA_STATIC_CONSTEXPR(bool, value =
    decltype((static_require_concept_test)(
      static_cast<T*>(0), static_cast<Property*>(0)))::value);
};

template <typename T, typename Property>
struct static_require_concept_trait<T, Property,
  typename enable_if<
    has_static_require_concept<T, Property>::value
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);
};

#endif // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)
      //   && defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
struct static_require_concept_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)
       //   && defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct static_require_concept_default :
  detail::static_require_concept_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct static_require_concept :
  static_require_concept_default<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_STATIC_REQUIRE_CONCEPT_HPP

namespace propria_require_concept_fn {

using propria::detail::decay;
using propria::detail::declval;
using propria::detail::enable_if;
using propria::is_applicable_property;
using propria::traits::require_concept_free;
using propria::traits::require_concept_member;
using propria::traits::static_require_concept;

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
      decay<Property>::type::is_requirable_concept
      &&
      static_require_concept<
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
      !static_require_concept<
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
      !static_require_concept<
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
    PROPRIA_NOEXCEPT_IF((
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
  require_concept PROPRIA_UNUSED_VARIABLE
    = propria_require_concept_fn::static_instance<>::instance;

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

template <typename T, typename Property>
struct is_nothrow_require_concept :
  detail::integral_constant<bool,
    propria_require_concept_fn::call_traits<T, void(Property)>::is_noexcept>
{
};

#if defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename Property>
constexpr bool is_nothrow_require_concept_v
  = is_nothrow_require_concept<T, Property>::value;

#endif // defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace propria

#endif // PROPRIA_REQUIRE_CONCEPT_HPP

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

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

//
// traits/require_member.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_REQUIRE_MEMBER_HPP
#define PROPRIA_TRAITS_REQUIRE_MEMBER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace propria {
namespace detail {

#if defined(PROPRIA_HAS_DECLTYPE) \
  && defined(PROPRIA_HAS_NOEXCEPT)

template <typename T, typename Property, typename = void>
struct require_member_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename Property>
struct require_member_trait<T, Property,
  typename void_type<
    decltype(declval<T>().require(declval<Property>()))
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    declval<T>().require(declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    declval<T>().require(declval<Property>())));
};

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)

template <typename T, typename Property>
struct require_member_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct require_member_default :
  detail::require_member_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct require_member :
  require_member_default<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_REQUIRE_MEMBER_HPP

//
// traits/require_free.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_REQUIRE_FREE_HPP
#define PROPRIA_TRAITS_REQUIRE_FREE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace propria {
namespace detail {

#if defined(PROPRIA_HAS_DECLTYPE) \
  && defined(PROPRIA_HAS_NOEXCEPT)

#if defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property, typename = void>
struct require_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename Property>
struct require_free_trait<T, Property,
  typename void_type<
    decltype(require(declval<T>(), declval<Property>()))
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    require(declval<T>(), declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    require(declval<T>(), declval<Property>())));
};

#else // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property,
  typename = decltype(require(declval<T>(), declval<Property>()))>
struct require_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    require(declval<T>(), declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    require(declval<T>(), declval<Property>())));
};

struct no_require {};

template <typename T, typename Property>
no_require require(T, Property);

template <typename T, typename Property>
struct require_free_trait<T, Property, no_require>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)

template <typename T, typename Property>
struct require_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct require_free_default :
  detail::require_free_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct require_free :
  require_free_default<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_REQUIRE_FREE_HPP

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

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

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

//
// prefer.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_PREFER_HPP
#define PROPRIA_PREFER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

//
// traits/prefer_free.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_PREFER_FREE_HPP
#define PROPRIA_TRAITS_PREFER_FREE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace propria {
namespace detail {

#if defined(PROPRIA_HAS_DECLTYPE) \
  && defined(PROPRIA_HAS_NOEXCEPT)

#if defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property, typename = void>
struct prefer_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename Property>
struct prefer_free_trait<T, Property,
  typename void_type<
    decltype(prefer(declval<T>(), declval<Property>()))
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    prefer(declval<T>(), declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    prefer(declval<T>(), declval<Property>())));
};

#else // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property,
  typename = decltype(prefer(declval<T>(), declval<Property>()))>
struct prefer_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    prefer(declval<T>(), declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    prefer(declval<T>(), declval<Property>())));
};

struct no_prefer {};

template <typename T, typename Property>
no_prefer prefer(T, Property);

template <typename T, typename Property>
struct prefer_free_trait<T, Property, no_prefer>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)

template <typename T, typename Property>
struct prefer_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct prefer_free_default :
  detail::prefer_free_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct prefer_free :
  prefer_free_default<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_PREFER_FREE_HPP

namespace propria_prefer_fn {

using propria::detail::decay;
using propria::detail::declval;
using propria::detail::enable_if;
using propria::is_applicable_property;
using propria::traits::prefer_free;
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
      decay<Property>::type::is_preferable
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
      decay<Property>::type::is_preferable
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
      decay<Property>::type::is_preferable
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

} // namespace propria_prefer_fn
namespace propria {
namespace {

static PROPRIA_CONSTEXPR const propria_prefer_fn::impl&
  prefer PROPRIA_UNUSED_VARIABLE
    = propria_prefer_fn::static_instance<>::instance;

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

#if defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

template <typename T, typename... Properties>
struct is_nothrow_prefer :
  detail::integral_constant<bool,
    propria_prefer_fn::call_traits<T, void(Properties...)>::is_noexcept>
{
};

#else // defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

template <typename T, typename P0 = void,
    typename P1 = void, typename P2 = void>
struct is_nothrow_prefer :
  detail::integral_constant<bool,
    propria_prefer_fn::call_traits<T, void(P0, P1, P2)>::is_noexcept>
{
};

template <typename T, typename P0, typename P1>
struct is_nothrow_prefer<T, P0, P1> :
  detail::integral_constant<bool,
    propria_prefer_fn::call_traits<T, void(P0, P1)>::is_noexcept>
{
};

template <typename T, typename P0>
struct is_nothrow_prefer<T, P0> :
  detail::integral_constant<bool,
    propria_prefer_fn::call_traits<T, void(P0)>::is_noexcept>
{
};

template <typename T>
struct is_nothrow_prefer<T> :
  detail::false_type
{
};

#endif // defined(PROPRIA_HAS_VARIADIC_TEMPLATES)

#if defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

template <typename T, typename PROPRIA_ELLIPSIS Properties>
constexpr bool is_nothrow_prefer_v
  = is_nothrow_prefer<T, Properties PROPRIA_ELLIPSIS>::value;

#endif // defined(PROPRIA_HAS_VARIABLE_TEMPLATES)

} // namespace propria

#endif // PROPRIA_PREFER_HPP

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

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

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

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

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

//
// traits/query_free.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef PROPRIA_TRAITS_QUERY_FREE_HPP
#define PROPRIA_TRAITS_QUERY_FREE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace propria {
namespace detail {

#if defined(PROPRIA_HAS_DECLTYPE) \
  && defined(PROPRIA_HAS_NOEXCEPT)

#if defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property, typename = void>
struct query_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

template <typename T, typename Property>
struct query_free_trait<T, Property,
  typename void_type<
    decltype(query(declval<T>(), declval<Property>()))
  >::type>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    query(declval<T>(), declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    query(declval<T>(), declval<Property>())));
};

#else // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

template <typename T, typename Property,
  typename = decltype(query(declval<T>(), declval<Property>()))>
struct query_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = true);

  using result_type = decltype(
    query(declval<T>(), declval<Property>()));

  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = noexcept(
    query(declval<T>(), declval<Property>())));
};

struct no_query {};

template <typename T, typename Property>
no_query query(T, Property);

template <typename T, typename Property>
struct query_free_trait<T, Property, no_query>
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_WORKING_EXPRESSION_SFINAE)

#else // defined(PROPRIA_HAS_DECLTYPE)
      //   && defined(PROPRIA_HAS_NOEXCEPT)

template <typename T, typename Property>
struct query_free_trait
{
  PROPRIA_STATIC_CONSTEXPR(bool, is_valid = false);
  PROPRIA_STATIC_CONSTEXPR(bool, is_noexcept = false);
};

#endif // defined(PROPRIA_HAS_DECLTYPE)
       //   && defined(PROPRIA_HAS_NOEXCEPT)

} // namespace detail
namespace traits {

template <typename T, typename Property, typename = void>
struct query_free_default :
  detail::query_free_trait<T, Property>
{
};

template <typename T, typename Property, typename = void>
struct query_free :
  query_free_default<T, Property>
{
};

} // namespace traits
} // namespace propria

#endif // PROPRIA_TRAITS_QUERY_FREE_HPP

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

#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

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
  query PROPRIA_UNUSED_VARIABLE
    = propria_query_fn::static_instance<>::instance;

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

#endif // PROPRIA_HPP
