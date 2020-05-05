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

#endif // PROPRIA_DETAIL_CONFIG_HPP
