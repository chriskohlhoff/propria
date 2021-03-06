//
// cpp11/can_require_free.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
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
  static constexpr bool is_requirable = true;
};

template <int>
struct object
{
  template <int N>
  friend constexpr object<N> require(const object&, prop<N>)
  {
    return object<N>();
  }
};

namespace propria {

template<int N, int M>
struct is_applicable_property<object<N>, prop<M> >
{
  static constexpr bool value = true;
};

} // namespace propria

int main()
{
  static_assert(propria::can_require<object<1>, prop<2>>::value, "");
  static_assert(propria::can_require<object<1>, prop<2>, prop<3>>::value, "");
  static_assert(propria::can_require<object<1>, prop<2>, prop<3>, prop<4>>::value, "");
  static_assert(propria::can_require<const object<1>, prop<2>>::value, "");
  static_assert(propria::can_require<const object<1>, prop<2>, prop<3>>::value, "");
  static_assert(propria::can_require<const object<1>, prop<2>, prop<3>, prop<4>>::value, "");
}
