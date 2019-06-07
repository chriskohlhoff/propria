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

#include <cstddef>
#include <functional>
#include <memory>
#include <new>
#include <propria.hpp>
#include <typeinfo>
#include <type_traits>

namespace execution {

//------------------------------------------------------------------------------

namespace detail {

struct archetypal_execute_function
{
  template <class SubExecutor>
  void operator()(SubExecutor);
};

} // namespace detail

//------------------------------------------------------------------------------

template <class E, class F = detail::archetypal_execute_function>
concept bool Executor = requires(const E& e, F f)
{
  e.execute(std::move(f));
};

//------------------------------------------------------------------------------

namespace detail {

template <class T, class P>
concept bool HasMemberFunctionQuery = requires(T t, P)
{
  t.query(P{});
};

template <class T, class P>
concept bool HasConstexprStaticMemberFunctionQuery =
  std::enable_if_t<
    (static_cast<void>(T::query(P{})), true),
    std::true_type
  >::value;

template <class T, class P>
concept bool HasConstexprStaticQueryVariable =
  std::enable_if_t<
    (static_cast<void>(P::template static_query_v<T>), true),
    std::true_type
  >::value;

} // namespace detail

//------------------------------------------------------------------------------

struct blocking_t
{
  template <class T>
  static constexpr bool is_applicable_property_v = Executor<T>;

  static constexpr bool is_requirable = false;
  static constexpr bool is_preferable = false;
  using polymorphic_query_result_type = blocking_t;

  struct always_t
  {
    template <class T>
    static constexpr bool is_applicable_property_v = Executor<T>;

    static constexpr bool is_requirable = true;
    static constexpr bool is_preferable = true;
    using polymorphic_query_result_type = blocking_t;

    template <
      class E,
      class P = always_t,
      class = std::enable_if_t<
        detail::HasConstexprStaticMemberFunctionQuery<E, P>
      >
    >
    static constexpr auto static_query_v = E::query(P{});

    static constexpr blocking_t value()
    {
      return blocking_t(always_t{});
    }
  };

  struct never_t
  {
    template <class T>
    static constexpr bool is_applicable_property_v = Executor<T>;

    static constexpr bool is_requirable = true;
    static constexpr bool is_preferable = true;
    using polymorphic_query_result_type = blocking_t;

    template <
      class E,
      class P = never_t,
      class = std::enable_if_t<
        detail::HasConstexprStaticMemberFunctionQuery<E, P>
      >
    >
    static constexpr auto static_query_v = E::query(P{});

    static constexpr blocking_t value()
    {
      return blocking_t(never_t{});
    }
  };

  struct possibly_t
  {
    template <class T>
    static constexpr bool is_applicable_property_v = Executor<T>;

    static constexpr bool is_requirable = true;
    static constexpr bool is_preferable = true;
    using polymorphic_query_result_type = blocking_t;

    template <
      class E,
      class P = possibly_t,
      std::enable_if_t<
        detail::HasConstexprStaticMemberFunctionQuery<E, P>
          || (!detail::HasMemberFunctionQuery<E, P>
              && !propria::can_query_v<E, always_t>
              && !propria::can_query_v<E, never_t>)
      >* = nullptr>
    static constexpr auto static_query()
    {
      if constexpr (detail::HasConstexprStaticMemberFunctionQuery<E, P>)
        return E::query(P{});
      else
        return P{};
    }

    template <class E, class T = decltype(static_query<E>())>
    static constexpr T static_query_v = static_query<E>();

    static constexpr blocking_t value() { return possibly_t{}; }
  };

  static constexpr possibly_t possibly{};
  static constexpr always_t always{};
  static constexpr never_t never{};

  constexpr blocking_t() : value_(-1) {}
  constexpr blocking_t(possibly_t) : value_(0) {}
  constexpr blocking_t(always_t) : value_(1) {}
  constexpr blocking_t(never_t) : value_(2) {}

  template <
    class E,
    class P = blocking_t,
    std::enable_if_t<
      detail::HasConstexprStaticMemberFunctionQuery<E, P>
        || (!detail::HasMemberFunctionQuery<E, P>
            && (detail::HasConstexprStaticQueryVariable<E, possibly_t>
              || detail::HasConstexprStaticQueryVariable<E, always_t>
              || detail::HasConstexprStaticQueryVariable<E, never_t>))
    >* = nullptr>
  static constexpr auto static_query()
  {
    if constexpr (detail::HasConstexprStaticMemberFunctionQuery<E, P>)
      return E::query(P{});
    else if constexpr (detail::HasConstexprStaticQueryVariable<E, possibly_t>)
      return possibly_t::template static_query_v<E>;
    else if constexpr (detail::HasConstexprStaticQueryVariable<E, always_t>)
      return always_t::template static_query_v<E>;
    else
      return never_t::template static_query_v<E>;
  }

  template <class E, class T = decltype(static_query<E>())>
  static constexpr T static_query_v = static_query<E>();

  template <class E> friend constexpr void query(const E&, possibly_t) = delete;
  template <class E> friend constexpr void query(const E&, always_t) = delete;
  template <class E> friend constexpr void query(const E&, never_t) = delete;

  template <class E,
    class Pk = std::conditional_t<
      propria::can_query_v<E, possibly_t>,
      possibly_t,
      std::conditional_t<
        propria::can_query_v<E, always_t>,
        always_t,
        never_t
      >
    >,
    std::enable_if_t<
      propria::can_query_v<E, possibly_t>
        || propria::can_query_v<E, always_t>
        || propria::can_query_v<E, never_t>
    >* = nullptr>
  friend constexpr blocking_t query(const E& e, blocking_t)
    noexcept(noexcept(propria::query(e, Pk{})))
  {
    return propria::query(e, Pk{});
  }

  friend constexpr bool operator==(const blocking_t& a, const blocking_t& b) noexcept
  {
    return a.value_ == b.value_;
  }

  friend constexpr bool operator!=(const blocking_t& a, const blocking_t& b) noexcept
  {
    return a.value_ != b.value_;
  }

private:
  int value_;
};

constexpr blocking_t blocking;
inline constexpr blocking_t::possibly_t blocking_t::possibly;
inline constexpr blocking_t::always_t blocking_t::always;
inline constexpr blocking_t::never_t blocking_t::never;

//------------------------------------------------------------------------------

struct occupancy_t
{
  template <class T>
  static constexpr bool is_applicable_property_v = Executor<T>;

  template <
    class E,
    class P = occupancy_t,
    class T = std::enable_if_t<
      (static_cast<void>(E::query(P{})), true),
      decltype(E::query(P{}))
    >
  >
  static constexpr T static_query_v = E::query(P{});

  using polymorphic_query_result_type = std::size_t;
};

constexpr occupancy_t occupancy;

//------------------------------------------------------------------------------

class bad_executor : std::exception
{
public:
  bad_executor() noexcept {}
  ~bad_executor() noexcept {}

  virtual const char* what() const noexcept
  {
    return "bad executor";
  }
};

//------------------------------------------------------------------------------

template <class... SupportableProperties>
class executor;

//------------------------------------------------------------------------------

namespace detail {

template <class Ex1, class Ex2>
constexpr bool is_executor_subset = false;

template <class... Props2>
constexpr bool is_executor_subset<executor<>, executor<Props2...>> = true;

template <class Props1, class... Props2>
constexpr bool is_executor_subset<executor<Props1>, executor<Props2...>>
  = (... || std::is_same_v<Props1, Props2>);

template <class... Props1, class... Props2>
constexpr bool is_executor_subset<executor<Props1...>, executor<Props2...>>
  = (... && is_executor_subset<executor<Props1>, executor<Props2...>>);

template <std::size_t I, class Prop, class... SupportableProperties>
struct convertible_prop
{
};

template <std::size_t I, class Prop, class... Tail>
struct convertible_prop<I, Prop, Prop, Tail...>
{
  using type = Prop;
  static constexpr std::size_t index = I;
};

template <std::size_t I, class Prop, class Head, class... Tail>
struct convertible_prop<I, Prop, Head, Tail...> :
  std::conditional_t<
    std::is_convertible_v<Prop, Head>,
    convertible_prop<I, Head, Head>,
    convertible_prop<I + 1, Prop, Tail...>>
{
};

template <class Prop>
concept bool RequirableProperty = Prop::is_requirable;

template <class Prop>
concept bool PreferableProperty = Prop::is_preferable;

} // namespace detail

//------------------------------------------------------------------------------

template <>
class executor<>
{
public:
  constexpr executor() noexcept
    : object_{},
      object_fns_(&object_fns_table<void>),
      target_(nullptr),
      target_fns_(&target_fns_table<void>)
  {
  }

  template <Executor Ex>
  executor(Ex ex)
    : target_fns_(&target_fns_table<Ex>)
  {
    if constexpr (propria::can_query_v<Ex, blocking_t>)
      blocking_ = propria::query(ex, blocking);
    if constexpr (sizeof(Ex) <= sizeof(object_))
    {
      object_fns_ = &object_fns_table<Ex>;
      target_ = new (object_) Ex(std::move(ex));
    }
    else
    {
      object_fns_ = &object_fns_table<std::shared_ptr<void>>;
      auto p = std::make_shared<Ex>(std::move(ex));
      target_ = p.get();
      new (object_) std::shared_ptr<void>(std::move(p));
    }
  }

  executor(const executor& other) noexcept
    : object_fns_(other.object_fns_),
      target_fns_(other.target_fns_),
      blocking_(other.blocking_)
  {
    object_fns_->copy(*this, other);
  }

  executor(executor&& other) noexcept
    : object_fns_(std::exchange(other.object_fns_, &object_fns_table<void>)),
      target_fns_(std::exchange(other.target_fns_, &target_fns_table<void>)),
      blocking_(std::exchange(other.blocking_, blocking_t{}))
  {
    object_fns_->move(*this, other);
    other.target_ = nullptr;
  }

  template <class... OtherSupportableProperties>
  executor(const executor<OtherSupportableProperties...>& other)
    : executor(static_cast<const executor&>(other))
  {
  }

  template <class... OtherSupportableProperties>
  executor(executor<OtherSupportableProperties...>&& other)
    : executor(static_cast<const executor&&>(other))
  {
  }

  ~executor()
  {
    object_fns_->destroy(*this);
  }

  executor& operator=(const executor& other) noexcept
  {
    if (this != &other)
    {
      object_fns_->destroy(*this);
      object_fns_ = other.object_fns_;
      target_fns_ = other.target_fns_;
      blocking_ = other.blocking_;
      object_fns_->copy(*this, other);
    }
    return *this;
  }

  executor& operator=(executor&& other) noexcept
  {
    if (this != &other)
    {
      object_fns_->destroy(*this);
      object_fns_ = std::exchange(other.object_fns_, &object_fns_table<void>);
      target_fns_ = std::exchange(other.target_fns_, &target_fns_table<void>);
      blocking_ = std::exchange(other.blocking_, blocking_t{});
      object_fns_->move(*this, other);
      other.target_ = nullptr;
    }
    return *this;
  }

  template <class F>
  void execute(F&& f) const
  {
    if (blocking_ == blocking.always)
    {
      function<std::decay_t<F>> erased_f(std::forward<F>(f));
      target_fns_->blocking_execute(*this, erased_f);
    }
    else
    {
      target_fns_->execute(*this,
          std::make_unique<function<std::decay_t<F>>>(std::forward<F>(f)));
    }
  }

  friend bool operator==(const executor& a, const executor& b) noexcept
  {
    if (a.target_ == b.target_)
      return true;
    if (a.target_ && !b.target_)
      return false;
    if (!a.target_ && b.target_)
      return false;
    if (a.target_fns_ != b.target_fns_)
      return false;
    return a.target_fns_->equal(a, b);
  }

  friend bool operator!=(const executor& a, const executor& b) noexcept
  {
    return !(a == b);
  }

  template <class Executor>
  Executor* target()
  {
    return static_cast<Executor*>(target_);
  }

  template <class Executor>
  const Executor* target() const
  {
    return static_cast<Executor*>(target_);
  }

  const std::type_info& target_type() const
  {
    return target_fns_->target_type();
  }

protected:
  struct function_base
  {
    virtual ~function_base() = default;
    virtual void invoke(const executor<>&) = 0;
    virtual void invoke(executor<>&&) = 0;
  };

  template <class F>
  struct function : function_base
  {
    F f_;
    explicit function(F f) : f_(std::move(f)) {}
    void invoke(const executor& ex) final override { f_(ex); }
    void invoke(executor&& ex) final override { f_(std::move(ex)); }
  };

  struct object_fns
  {
    void (*destroy)(executor&);
    void (*copy)(executor&, const executor&);
    void (*move)(executor&, executor&);
    const void* (*target)(const executor&);
  };

  template <class Ex>
  Ex& object()
  {
    return *static_cast<Ex*>(static_cast<void*>(object_));
  }

  template <class Ex>
  const Ex& object() const
  {
    return *static_cast<const Ex*>(static_cast<const void*>(object_));
  }

  template <class Obj>
  static void destroy_fn(executor& ex)
  {
    if constexpr (!std::is_same_v<Obj, void>)
      ex.object<Obj>().~Obj();
    else
      (void)ex;
  }

  template <class Obj>
  static void copy_fn(executor& ex1, const executor& ex2)
  {
    if constexpr (!std::is_same_v<Obj, void>)
    {
      new (ex1.object_) Obj(ex2.object<Obj>());
      if constexpr (std::is_same_v<Obj, std::shared_ptr<void>>)
        ex1.target_ = ex2.target_;
      else
        ex1.target_ = &ex1.object<Obj>();
    }
    else
      (void)ex1, (void)ex2;
  }

  template <class Obj>
  static void move_fn(executor& ex1, executor& ex2)
  {
    if constexpr (!std::is_same_v<Obj, void>)
    {
      new (ex1.object_) Obj(std::move(ex2.object<Obj>()));
      if constexpr (std::is_same_v<Obj, std::shared_ptr<void>>)
        ex1.target_ = ex2.target_;
      else
        ex1.target_ = &ex1.object<Obj>();
      ex2.object<Obj>().~Obj();
    }
    else
      (void)ex1, (void)ex2;
  }

  template <class Obj>
  static const void* target_fn(const executor& ex)
  {
    if constexpr (!std::is_same_v<Obj, void>)
    {
      if constexpr (std::is_same_v<Obj, std::shared_ptr<void>>)
        return ex.object<Obj>().get();
      else
        return &ex.object<Obj>();
    }
    else
    {
      (void)ex;
      return nullptr;
    }
  }

  template <class Obj>
  static constexpr object_fns object_fns_table = {
    &executor::destroy_fn<Obj>,
    &executor::copy_fn<Obj>,
    &executor::move_fn<Obj>,
    &executor::target_fn<Obj>
  };

  struct target_fns
  {
    const std::type_info& (*target_type)();
    bool (*equal)(const executor&, const executor&);
    void (*execute)(const executor&, std::unique_ptr<function_base>);
    void (*blocking_execute)(const executor&, function_base&);
  };

  template <class Ex>
  static const std::type_info& target_type_fn()
  {
    return typeid(Ex);
  }

  template <class Ex>
  static bool equal_fn(const executor& ex1, const executor& ex2)
  {
    if constexpr (!std::is_same_v<Ex, void>)
    {
      return *ex1.target<Ex>() == *ex2.target<Ex>();
    }
    else
    {
      (void)ex1, (void)ex2;
      return true;
    }
  }

  template <class Ex>
  static void execute_fn(const executor& ex, std::unique_ptr<function_base> f)
  {
    if constexpr (!std::is_same_v<Ex, void>)
    {
      ex.target<Ex>()->execute(
          [f = std::move(f)](auto sub_ex) mutable
          {
            f->invoke(executor(sub_ex));
          });
    }
    else
    {
      (void)ex, (void)f;
      throw bad_executor();
    }
  }

  template <class Ex>
  static void blocking_execute_fn(const executor& ex, function_base& f)
  {
    if constexpr (!std::is_same_v<Ex, void>)
    {
      ex.target<Ex>()->execute(
          [&ex, &f](auto sub_ex)
          {
            if constexpr (std::is_same_v<Ex, decltype(sub_ex)>)
            {
              if (*ex.target<Ex>() == sub_ex)
                f.invoke(ex);
              else
                f.invoke(executor(sub_ex));
            }
            else
              f.invoke(executor(sub_ex));
          });
    }
    else
    {
      (void)ex, (void)f;
      throw bad_executor();
    }
  }

  template <class Ex>
  static constexpr target_fns target_fns_table = {
    &executor::target_type_fn<Ex>,
    &executor::equal_fn<Ex>,
    &executor::execute_fn<Ex>,
    &executor::blocking_execute_fn<Ex>
  };

  template <class Ex, class Prop>
  static void* query_fn(const void* ex, const void* prop)
  {
    if constexpr (!std::is_same_v<Ex, void>)
    {
      if constexpr (detail::RequirableProperty<Prop>
          || detail::PreferableProperty<Prop>
          || propria::can_query_v<Ex, Prop>)
      {
        if constexpr (std::is_same_v<typename Prop::polymorphic_query_result_type, void>)
        {
          propria::query(*static_cast<const Ex*>(ex),
              *static_cast<const Prop*>(prop));
          return nullptr;
        }
        else
        {
          return new typename Prop::polymorphic_query_result_type(
              propria::query(*static_cast<const Ex*>(ex),
                *static_cast<const Prop*>(prop)));
        }
      }
      else
      {
        (void)ex, (void)prop;
        return new typename Prop::polymorphic_query_result_type();
      }
    }
    else
    {
      (void)ex, (void)prop;
      throw bad_executor();
    }
  }

  template <class Poly, class Ex, class Prop>
  static Poly require_fn(const void* ex, const void* prop)
  {
    if constexpr (!std::is_same_v<Ex, void>)
    {
      if constexpr (detail::RequirableProperty<Prop>)
      {
        return propria::require(
            *static_cast<const Ex*>(ex),
            *static_cast<const Prop*>(prop));
      }
      else
      {
        (void)ex, (void)prop;
        return {};
      }
    }
    else
    {
      (void)ex, (void)prop;
      throw bad_executor();
    }
  }

  template <class Poly, class Ex, class Prop>
  static Poly prefer_fn(const void* ex, const void* prop)
  {
    if constexpr (!std::is_same_v<Ex, void>)
    {
      if constexpr (detail::PreferableProperty<Prop>)
      {
        return propria::prefer(
            *static_cast<const Ex*>(ex),
            *static_cast<const Prop*>(prop));
      }
      else
      {
        (void)ex, (void)prop;
        return {};
      }
    }
    else
    {
      (void)ex, (void)prop;
      throw bad_executor();
    }
  }

private:
  template <class...> friend class executor;
  alignas (std::max_align_t) unsigned char object_[sizeof(std::shared_ptr<void>)];
  const object_fns* object_fns_;
  void* target_;
  const target_fns* target_fns_;
  blocking_t blocking_;
};

template <class... SupportableProperties>
class executor : public executor<>
{
private:
  template <class Prop>
  using prop_type =
    typename detail::convertible_prop<0, Prop, SupportableProperties...>::type;

  template <class Prop>
  static constexpr std::size_t prop_index =
    detail::convertible_prop<0, Prop, SupportableProperties...>::index;

  template <class Prop>
  using prop_query_result_type =
    typename detail::convertible_prop<0, Prop,
      SupportableProperties...>::type::polymorphic_query_result_type;

public:
  constexpr executor()
    : executor<>(),
      prop_fns_(prop_fns_table<void>)
  {
  }

  template <Executor Ex>
  executor(Ex ex)
    : executor<>(std::move(ex)),
      prop_fns_(prop_fns_table<Ex>)
  {
  }

  template <class... OtherSupportableProperties,
    std::enable_if_t<
      detail::is_executor_subset<executor, executor<OtherSupportableProperties...>>
    >* = nullptr>
  executor(executor<OtherSupportableProperties...> other)
    : prop_fns_(prop_fns_table<decltype(other)>)
  {
    object_fns_ = &object_fns_table<std::shared_ptr<void>>;
    target_fns_ = other.target_fns_;
    blocking_ = other.blocking_;
    auto p = std::make_shared<decltype(other)>(std::move(other));
    target_ = p->template target<void>();
    new (object_) std::shared_ptr<void>(std::move(p));
  }

  template <class... OtherSupportableProperties,
    std::enable_if_t<
      !detail::is_executor_subset<executor, executor<OtherSupportableProperties...>>
    >* = nullptr>
  executor(executor<OtherSupportableProperties...>) = delete;

  executor(const executor& other) noexcept
    : executor<>(static_cast<const executor<>&>(other)),
      prop_fns_(other.prop_fns_)
  {
  }

  executor(executor&& other) noexcept
    : executor<>(std::move(static_cast<executor<>&>(other))),
      prop_fns_(std::exchange(other.prop_fns_, prop_fns_table<void>))
  {
  }

  executor& operator=(const executor& other) noexcept
  {
    if (this != &other)
    {
      prop_fns_ = other.prop_fns_;
      executor<>::operator=(other);
    }
    return *this;
  }

  executor& operator=(executor&& other) noexcept
  {
    if (this != &other)
    {
      prop_fns_ = std::exchange(other.prop_fns_, prop_fns_table<void>);
      executor<>::operator=(std::move(other));
    }
    return *this;
  }

  template <class Prop>
  prop_query_result_type<Prop> query(const Prop& p) const
  {
    prop_type<Prop> p2(p);
    if constexpr (std::is_same_v<prop_query_result_type<Prop>, void>)
    {
      prop_fns_[prop_index<Prop>].query(object_fns_->target(*this), &p2);
    }
    else
    {
      return *std::unique_ptr<prop_query_result_type<Prop>>(
          static_cast<prop_query_result_type<Prop>*>(
            prop_fns_[prop_index<Prop>].query(object_fns_->target(*this), &p2)));
    }
  }

  template <class Prop,
    std::enable_if_t<
      prop_type<Prop>::is_requirable
    >* = nullptr>
  executor require(const Prop& p) const
  {
    return prop_fns_[prop_index<Prop>].require(object_fns_->target(*this), &p);
  }

  template <class Prop,
    std::enable_if_t<
      prop_type<Prop>::is_preferable
    >* = nullptr>
  friend executor prefer(const executor& ex, const Prop& p)
  {
    return ex.prop_fns_[prop_index<Prop>].prefer(ex.object_fns_->target(ex), &p);
  }

private:
  struct prop_fns
  {
    void* (*query)(const void*, const void*);
    executor (*require)(const void*, const void*);
    executor (*prefer)(const void*, const void*);
  };

  template <class Ex>
  static constexpr prop_fns prop_fns_table[] = {
    {
      &executor<>::query_fn<Ex, SupportableProperties>,
      &executor<>::require_fn<executor, Ex, SupportableProperties>,
      &executor<>::prefer_fn<executor, Ex, SupportableProperties>
    }...
  };

  const prop_fns* prop_fns_;
};

//------------------------------------------------------------------------------

} // namespace execution

#endif // EXECUTION_HPP
