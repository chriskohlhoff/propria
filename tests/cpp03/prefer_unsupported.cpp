//
// cpp03/prefer_unsupported.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "propria/prefer.hpp"
#include <cassert>

template <int>
struct prop
{
  static const bool is_preferable = true;
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

} // namespace propria

int main()
{
  object<1> o1;
  const object<1>& o2 = propria::prefer(o1, prop<1>());
  assert(&o1 == &o2);
  (void)o2;

  const object<1> o3;
  const object<1>& o4 = propria::prefer(o3, prop<1>());
  assert(&o3 == &o4);
  (void)o4;
}
