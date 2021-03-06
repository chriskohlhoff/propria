//
// cpp03/query_free.cpp
// ~~~~~~~~~~~~~~~~~~~~
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
  friend int query(const object&, prop) { return 123; }
};

namespace propria {

template<>
struct is_applicable_property<object, prop>
{
  static const bool value = true;
};

namespace traits {

template<>
struct query_free<object, prop>
{
  static const bool is_valid = true;
  static const bool is_noexcept = true;
  typedef int result_type;
};

} // namespace traits
} // namespace propria

int main()
{
  object o1 = {};
  int result1 = propria::query(o1, prop());
  assert(result1 == 123);
  (void)result1;

  const object o2 = {};
  int result2 = propria::query(o2, prop());
  assert(result2 == 123);
  (void)result2;
}
