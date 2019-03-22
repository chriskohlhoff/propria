//
// cpp11/can_query_not_applicable_free.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
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

int main()
{
  static_assert(!propria::can_query<object, prop>::value, "");
  static_assert(!propria::can_query<const object, prop>::value, "");
}
