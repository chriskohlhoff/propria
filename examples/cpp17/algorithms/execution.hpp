//
// execution.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXECUTION_HPP
#define EXECUTION_HPP

#include <memory>
#include <propria.hpp>
#include <typeinfo>
#include <type_traits>

namespace execution {

//------------------------------------------------------------------------------

struct executor_concept_t
{
  template <typename>
  static constexpr bool is_applicable_property_v = true;

  template <
    typename E,
    typename P = executor_concept_t,
    typename T = std::enable_if_t<
      (static_cast<void>(E::query(P{})), true),
      decltype(E::query(P{}))
    >
  >
  static constexpr T static_query_v = E::query(P{});
};

constexpr executor_concept_t executor_concept;

//------------------------------------------------------------------------------

struct oneway_t
{
  template <typename T>
  static constexpr bool is_applicable_property_v
    = propria::can_query_v<T, executor_concept_t>;

  static constexpr bool is_requirable_concept = true;

  template <
    typename E,
    typename P = oneway_t,
    typename T = decltype(propria::query(std::declval<E>(), executor_concept))
  >
  static constexpr bool static_query_v = std::is_convertible_v<T, P>;

  static constexpr bool value() { return true; }

  using polymorphic_query_result_type = bool;

  template<class... SupportableProperties>
  class polymorphic_executor_type;
};

constexpr oneway_t oneway;

//------------------------------------------------------------------------------

template<class... SupportableProperties>
class oneway_t::polymorphic_executor_type
{
public:
  template <class Ex,
    std::enable_if_t<
      oneway_t::static_query_v<Ex>
    >* = nullptr>
  polymorphic_executor_type(Ex ex)
    : impl_(std::make_shared<impl<Ex>>(std::move(ex))),
      fns_(make_fns<Ex>())
  {
  }

  static constexpr oneway_t query(executor_concept_t)
  {
    return {};
  }

  template <class F>
  void execute(F f) const
  {
    impl_->execute(std::make_unique<function<F>>(std::move(f)));
  }

  void* target()
  {
    return impl_->target_;
  }

  const void* target() const
  {
    return impl_->target_;
  }

  const std::type_info& target_type() const
  {
    return *impl_->target_type_;
  }

  template <class Prop>
  typename Prop::polymorphic_query_result_type query(const Prop& p) const
  {
    return *std::unique_ptr<typename Prop::polymorphic_query_result_type>(
        static_cast<typename Prop::polymorphic_query_result_type*>(
          fns_->query[this->prop_index<0, Prop, SupportableProperties...>()](
            impl_.get(), &p)));
  }

  template <class Prop>
  typename Prop::template polymorphic_executor_proxy<polymorphic_executor_type>
  require_concept(const Prop&) const
  {
    return typename Prop::template polymorphic_executor_proxy<
      polymorphic_executor_type>(*this,
          fns_->proxy[this->prop_index<0, Prop, SupportableProperties...>()]);
  }

private:
  struct function_base
  {
    virtual ~function_base() = default;
    virtual void invoke() = 0;
  };

  template <class F>
  struct function : function_base
  {
    F f_;

    explicit function(F f)
      : f_(std::move(f))
    {
    }

    void invoke() override
    {
      f_();
    }
  };

  struct impl_base
  {
    virtual ~impl_base() = default;
    virtual void execute(std::unique_ptr<function_base> f) const = 0;
    void* target_;
    const std::type_info* target_type_;
  };

  template <class Ex>
  struct impl : impl_base
  {
    Ex ex_;

    explicit impl(Ex ex)
      : ex_(std::move(ex))
    {
      this->target_ = &ex_;
      this->target_type_ = &typeid(Ex);
    }

    void execute(std::unique_ptr<function_base> f) const override
    {
      ex_.execute([f = std::move(f)]{ f->invoke(); });
    }
  };

  template <std::size_t I, class Prop, class Head, class... Tail>
  static constexpr std::size_t prop_index()
  {
    if constexpr (std::is_same_v<Prop, Head>)
      return I;
    else
      return polymorphic_executor_type::prop_index<I + 1, Prop, Tail...>();
  }

  using query_fn_type = void* (*)(const impl_base*, const void*);

  template <class Ex, class Prop>
  static void* query_fn(const impl_base* ex, const void* prop)
  {
    if constexpr (propria::can_query_v<Ex, Prop>)
    {
      return new typename Prop::polymorphic_query_result_type(
          propria::query(*static_cast<const Ex*>(ex->target_),
            *static_cast<const Prop*>(prop)));
    }
    else
    {
      return new typename Prop::polymorphic_query_result_type();
    }
  }

  template <class Ex, class Prop>
  static void* get_proxy(Prop*, ...)
  {
    return nullptr;
  }

  template <class Ex, class Prop>
  static void* get_proxy(Prop*,
      typename Prop::template polymorphic_executor_proxy<
        polymorphic_executor_type>*)
  {
    return Prop::template polymorphic_executor_proxy<
      polymorphic_executor_type>::template get_proxy<Ex>();
  }

  struct fns
  {
    query_fn_type query[sizeof...(SupportableProperties) + 1];
    void* proxy[sizeof...(SupportableProperties) + 1];
  };

  template <class Ex>
  fns* make_fns()
  {
    static fns f = {
      {
        &polymorphic_executor_type::query_fn<Ex, SupportableProperties>...
      },
      {
        polymorphic_executor_type::get_proxy<Ex>(
          static_cast<SupportableProperties*>(nullptr), nullptr)...
      }
    };
    return &f;
  }

  std::shared_ptr<impl_base> impl_;
  fns* fns_;
};

//------------------------------------------------------------------------------

template <class ExecutorConcept, class... SupportableProperties>
using executor = typename ExecutorConcept::template polymorphic_executor_type<
  ExecutorConcept, SupportableProperties...>;

//------------------------------------------------------------------------------

struct occupancy_t
{
  template <typename T>
  static constexpr bool is_applicable_property_v
    = propria::can_query_v<T, executor_concept_t>;

  template <
    typename E,
    typename P = occupancy_t,
    typename T = std::enable_if_t<
      (static_cast<void>(E::query(P{})), true),
      decltype(E::query(P{}))
    >
  >
  static constexpr T static_query_v = E::query(P{});

  using polymorphic_query_result_type = std::size_t;
};

constexpr occupancy_t occupancy;

//------------------------------------------------------------------------------

} // namespace execution

#endif // EXECUTION_HPP
