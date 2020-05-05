//
// cpp11/can_prefer_not_preferable_member.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "propria/prefer.hpp"
#include <cassert>

template <int>
struct prop
{
  static constexpr bool is_preferable = false;
};

template <int>
struct object
{
  template <int N>
  constexpr object<N> require(prop<N>) const
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
  static_assert(!propria::can_prefer<object<1>, prop<2>>::value, "");
  static_assert(!propria::can_prefer<object<1>, prop<2>, prop<3>>::value, "");
  static_assert(!propria::can_prefer<object<1>, prop<2>, prop<3>, prop<4>>::value, "");
  static_assert(!propria::can_prefer<const object<1>, prop<2>>::value, "");
  static_assert(!propria::can_prefer<const object<1>, prop<2>, prop<3>>::value, "");
  static_assert(!propria::can_prefer<const object<1>, prop<2>, prop<3>, prop<4>>::value, "");
}
