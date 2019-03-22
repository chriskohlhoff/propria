//
// cpp11/can_require_concept_free.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "propria/require_concept.hpp"
#include <cassert>

template <int>
struct prop
{
  static constexpr bool is_requirable_concept = true;
};

template <int>
struct object
{
  template <int N>
  friend constexpr object<N> require_concept(const object&, prop<N>)
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
  static_assert(propria::can_require_concept<object<1>, prop<2>>::value, "");
  static_assert(propria::can_require_concept<const object<1>, prop<2>>::value, "");
}
