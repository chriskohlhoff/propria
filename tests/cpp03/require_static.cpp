//
// cpp03/require_static.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "propria/require.hpp"
#include <cassert>

template <int>
struct prop
{
  static const bool is_requirable = true;
};

template <int>
struct object
{
};

namespace propria {

template<int N, int M>
struct is_applicable_property<object<N>, prop<M> >
{
  static const bool value = true;
};

namespace traits {

template<int N>
struct static_require<object<N>, prop<N> >
{
  static const bool is_valid = true;
};

} // namespace traits
} // namespace propria

int main()
{
  object<1> o1 = {};
  object<1> o2 = propria::require(o1, prop<1>());
  object<1> o3 = propria::require(o1, prop<1>(), prop<1>());
  object<1> o4 = propria::require(o1, prop<1>(), prop<1>(), prop<1>());
  (void)o2;
  (void)o3;
  (void)o4;

  const object<1> o5 = {};
  object<1> o6 = propria::require(o5, prop<1>());
  object<1> o7 = propria::require(o5, prop<1>(), prop<1>());
  object<1> o8 = propria::require(o5, prop<1>(), prop<1>(), prop<1>());
  (void)o6;
  (void)o7;
  (void)o8;
}
