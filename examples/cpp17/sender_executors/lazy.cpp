#include "execution.hpp"
#include <tuple>

//------------------------------------------------------------------------------

template <execution::Signature... Sigs>
struct signatures {};

inline constexpr struct probe_t {} probe;

namespace execution {

template <Signature... Sigs>
struct receiver_traits< ::probe_t, Sigs...>
{
  using receiver_type = probe_t;

  template <class SenderLauncher, class Receiver, class... Args>
  static auto connect(SenderLauncher&&, Receiver&&, Args&&...)
  {
    return signatures<Sigs...>{};
  }
};

} // namespace execution

//------------------------------------------------------------------------------

inline constexpr struct lazy_t {} lazy;

template <class SenderLauncher, class ArgTuple, execution::Signature... Sigs>
struct lazy_task
{
  SenderLauncher sender;
  ArgTuple args;

  template <class Predecessor, execution::Receiver<Sigs...> R>
  auto operator()(Predecessor pred, R&& r)
  {
    return execution::connect<Sigs...>(
        [pred = std::move(pred)](
            execution::Handler<Sigs...> h,
            SenderLauncher&& s,
            ArgTuple&& args
          ) mutable
        {
          pred(
              [s = std::move(s), h = std::move(h), args = std::move(args)]() mutable
              {
                std::apply(
                    [&](auto&&... a)
                    {
                      std::invoke(
                          std::move(s),
                          std::move(h),
                          std::forward<decltype(a)>(a)...);
                    },
                    std::move(args)
                  );
              }
            );
        },
        std::forward<R>(r),
        std::move(sender),
        std::move(args)
      );
  }

  template <execution::Receiver<Sigs...> R>
  auto operator()(R&& r)
  {
    return std::apply(
        [&](auto&&... a)
        {
          return execution::connect<Sigs...>(
              std::move(sender),
              std::forward<R>(r),
              std::forward<decltype(a)>(a)...
            );
        },
        std::move(args)
      );
  }
};

namespace execution {

template <Signature... Sigs>
struct receiver_traits< ::lazy_t, Sigs...>
{
  using receiver_type = lazy_t;

  template <class SenderLauncher, class Receiver, class... Args>
  static auto connect(SenderLauncher s, Receiver&&, Args... args)
  {
    return lazy_task<SenderLauncher, std::tuple<Args...>, Sigs...>{
      std::move(s), std::make_tuple(std::move(args)...)
    };
  }
};

} // namespace execution

//------------------------------------------------------------------------------

template <class Blocking, std::size_t Padding>
struct inline_executor
{
  static constexpr execution::blocking_t query(execution::blocking_t)
  {
    return Blocking{};
  }

  inline_executor<execution::blocking_t::possibly_t, Padding> require(execution::blocking_t::possibly_t) const
  {
    return {};
  }

  inline_executor<execution::blocking_t::always_t, Padding> require(execution::blocking_t::always_t) const
  {
    return {};
  }

  friend constexpr bool operator==(const inline_executor&, const inline_executor&) noexcept
  {
    return true;
  }

  friend constexpr bool operator!=(const inline_executor&, const inline_executor&) noexcept
  {
    return false;
  }

  template <execution::Receiver<void(inline_executor)> R>
  auto execute(R&& r) const
  {
    return execution::connect<void(inline_executor)>(
        [](execution::Handler<void(inline_executor)> h, const inline_executor& ex)
        {
          std::printf("starting invoke\n");
          std::invoke(h, ex);
          std::printf("ended invoke\n");
        },
        std::forward<R>(r),
        *this
      );
  }

  unsigned char padding_[Padding];
};

//------------------------------------------------------------------------------

template <class F, execution::Receiver<void()> R>
auto call(F f, R&& r)
{
  return execution::connect<void()>(
      [](execution::Handler<void()> h, F f)
      {
        f();
        h();
      },
      std::forward<R>(r),
      std::move(f)
    );
}

//------------------------------------------------------------------------------

template <class T, execution::Receiver<void(T)> R>
auto just(T value, R&& r)
{
  return execution::connect<void(T)>(
      [](auto h, auto v) mutable
      {
        std::invoke(std::move(h), std::move(v));
      },
      std::forward<R>(r),
      value
    );
}

template <class Predecessor, execution::Receiver<void()> R>
auto drop_args(Predecessor p, R&& r)
{
  return execution::connect<void()>(
      [](
          execution::Handler<void()> h,
          Predecessor pred
        ) mutable
      {
        pred(
            [h = std::move(h)](auto&&...) mutable
            {
              std::invoke(std::move(h));
            }
          );
      },
      std::forward<R>(r),
      std::move(p)
    );
}

template <execution::Signature... Sigs, class Predecessor, execution::Receiver<Sigs...> R>
static auto times_2_impl(signatures<Sigs...>, Predecessor p, R&& r)
{
  return execution::connect<Sigs...>(
      [](
          execution::Handler<Sigs...> h,
          Predecessor pred
        ) mutable
      {
        pred(
            [h = std::move(h)](auto v) mutable
            {
              std::invoke(std::move(h), v * 2);
            }
          );
      },
      std::forward<R>(r),
      std::move(p)
    );
}

struct times_2_t
{
  template <class Predecessor, class Receiver>
  auto operator()(Predecessor p, Receiver&& r) const
  {
    return times_2_impl(decltype(p(probe)){}, std::move(p), std::forward<Receiver>(r));
  }
} times_2;

//------------------------------------------------------------------------------

#include <cstdio>

int main()
{
  inline_executor<execution::blocking_t::possibly_t, 1> ex1;

  auto x = ex1.execute(lazy);
  x([](auto){ std::printf("hello\n"); });

  auto x1 = call([]{ std::printf("a\n"); }, lazy);
  auto x2 = call([]{ std::printf("b\n"); }, lazy);
  auto x3 = call([]{ std::printf("c\n"); }, lazy);

  auto c = drop_args(x, lazy);
  auto c0 = x1(c, lazy);
  auto c1 = x2(c0, lazy);
  auto c2 = x3(c1, lazy);
  c2([]{});

  auto a1 = just(42, lazy);
  auto a2 = times_2(a1, lazy);
  a2([](auto v){ std::printf("%d\n", v); });
}
