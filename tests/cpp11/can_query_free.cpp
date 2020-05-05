//
// cpp11/can_query_free.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "propria/query.hpp"
#include <cassert>

struct prop
{
};

struct object
{
  friend constexpr int query(const object&, prop) { return 123; }
};

namespace propria {

template<>
struct is_applicable_property<object, prop>
{
  static constexpr bool value = true;
};

} // namespace propria

int main()
{
  static_assert(propria::can_query<object, prop>::value, "");
  static_assert(propria::can_query<const object, prop>::value, "");
}
