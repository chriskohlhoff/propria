//
// cpp03/can_query_not_applicable_static.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
};

namespace propria {
namespace traits {

template<>
struct query_static<object, prop>
{
  static const bool is_valid = true;
  static const bool is_noexcept = true;
  typedef int result_type;
  static int value() { return 123; }
};

} // namespace traits
} // namespace propria

int main()
{
  assert((!propria::can_query<object, prop>::value));
  assert((!propria::can_query<const object, prop>::value));
}
