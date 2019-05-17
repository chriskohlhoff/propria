//
// fill_4.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_4_0.txt or copy at http://www.boost.org/LICENSE_4_0.txt)
//

#ifndef FILL_4_HPP
#define FILL_4_HPP

#include "extension_property.hpp"

namespace algorithm {

//------------------------------------------------------------------------------
// fill #4: type-specific algorithm with default and polymorphic support

namespace extensions {

template <class ForwardIt, class T>
struct fill_4_t : extension_property<fill_4_t<ForwardIt, T>>
{
  template <class Ex>
  struct dflt
  {
    Ex ex;

    static constexpr bool query(fill_4_t)
    {
      return true;
    }

    void fill_4(ForwardIt first, ForwardIt last, const T& value) const
    {
      for (ForwardIt iter = first; iter != last; ++iter)
        ex.execute([&elem = *iter, value]{ elem = value; });
    }
  };

  template <class Ex,
    std::enable_if_t<
      std::is_convertible_v<
        decltype(propria::query(std::declval<Ex>(), execution::executor_concept)),
        execution::oneway_t
      >
    >* = nullptr>
  friend auto require_concept(const Ex& ex, fill_4_t)
  {
    return dflt<Ex>{ex};
  }

  using polymorphic_query_result_type = bool;

  template <class PolyEx>
  class polymorphic_executor_proxy
  {
  public:
    template <class Ex>
    static void* get_proxy()
    {
      static proxy p =
      {
        [](const void* ex, ForwardIt first, ForwardIt last, const T& value)
        {
          const Ex* ex2 = static_cast<const Ex*>(ex);
          propria::require_concept(*ex2, fill_4_t{}).fill_4(first, last, value);
        }
      };
      return &p;
    }

    explicit polymorphic_executor_proxy(PolyEx ex, void* p)
      : ex_(ex), proxy_(static_cast<proxy*>(p))
    {
    }

    static constexpr bool query(fill_4_t)
    {
      return true;
    }

    void fill_4(ForwardIt first, ForwardIt last, const T& value) const
    {
      proxy_->fill_4(ex_.target(), first, last, value);
    }

  private:
    struct proxy
    {
      void (*fill_4)(const void* ex, ForwardIt first,
          ForwardIt last, const T& value);
    };

    PolyEx ex_;
    proxy* proxy_;
  };
};

template <class ForwardIt, class T>
constexpr fill_4_t<ForwardIt, T> fill_4;

} // namespace extensions

template <class ForwardIt, class T>
void fill_4(ForwardIt first, ForwardIt last, const T& value)
{
  for (ForwardIt iter = first; iter != last; ++iter)
    *iter = value;
}

template <class Ex, class ForwardIt, class T>
void fill_4(const Ex& ex, ForwardIt first, ForwardIt last, const T& value)
{
  propria::require_concept(ex,
      extensions::fill_4<ForwardIt, T>
    ).fill_4(first, last, value);
}

//------------------------------------------------------------------------------

} // namespace algorithm

#endif // FILL_4_HPP
