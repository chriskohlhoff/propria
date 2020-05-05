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
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "propria/detail/config.hpp"

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
