//
// cpp03/can_query_member.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~
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
  int query(prop) const { return 123; }
};

namespace propria {

template<>
struct is_applicable_property<object, prop>
{
  static const bool value = true;
};

namespace traits {

template<>
struct query_member<object, prop>
{
  static const bool is_valid = true;
  static const bool is_noexcept = true;
  typedef int result_type;
};

} // namespace traits
} // namespace propria

int main()
{
  assert((propria::can_query<object, prop>::value));
  assert((propria::can_query<const object, prop>::value));
}
