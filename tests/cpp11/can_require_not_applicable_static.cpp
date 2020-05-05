//
// cpp11/can_require_not_applicable_static.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
};

namespace propria {
namespace traits {

template<int N>
struct require_static<object<N>, prop<N> >
{
  static constexpr bool is_valid = true;
};

} // namespace traits
} // namespace propria

int main()
{
  static_assert(!propria::can_require<object<1>, prop<1>>::value, "");
  static_assert(!propria::can_require<object<1>, prop<1>, prop<1>>::value, "");
  static_assert(!propria::can_require<object<1>, prop<1>, prop<1>, prop<1>>::value, "");
  static_assert(!propria::can_require<const object<1>, prop<1>>::value, "");
  static_assert(!propria::can_require<const object<1>, prop<1>, prop<1>>::value, "");
  static_assert(!propria::can_require<const object<1>, prop<1>, prop<1>, prop<1>>::value, "");
}
