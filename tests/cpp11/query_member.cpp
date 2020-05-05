//
// cpp11/query_member.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
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
  constexpr int query(prop) const { return 123; }
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
  object o1;
  int result1 = propria::query(o1, prop());
  assert(result1 == 123);
  (void)result1;

  const object o2;
  int result2 = propria::query(o2, prop());
  assert(result2 == 123);
  (void)result2;

  constexpr object o3;
  constexpr int result3 = propria::query(o3, prop());
  assert(result3 == 123);
  (void)result3;
}
