//
// cpp03/require_concept_static.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "propria/require_concept.hpp"
#include <cassert>

template <int>
struct prop
{
  static const bool is_requirable_concept = true;
};

template <int>
struct object
{
};

namespace propria {

template<int N, int M>
struct is_applicable_property<object<N>, prop<M> >
{
  static const bool value = true;
};

namespace traits {

template<int N>
struct require_concept_static<object<N>, prop<N> >
{
  static const bool is_valid = true;
};

} // namespace traits
} // namespace propria

int main()
{
  object<1> o1;
  const object<1>& o2 = propria::require_concept(o1, prop<1>());
  assert(&o1 == &o2);
  (void)o2;

  const object<1> o3;
  const object<1>& o4 = propria::require_concept(o3, prop<1>());
  assert(&o3 == &o4);
  (void)o4;
}
