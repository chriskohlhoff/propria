#include "execution.hpp"
#include <cstdio>
#include <cassert>
#include <future>

//------------------------------------------------------------------------------

struct inline_executor
{
  friend bool operator==(const inline_executor&, const inline_executor&) noexcept
  {
    return true;
  }

  friend bool operator!=(const inline_executor&, const inline_executor&) noexcept
  {
    return false;
  }

  template <execution::Receiver<void(inline_executor)> R>
  auto execute(R&& r) const
  {
    return execution::connect<void(inline_executor)>(
        [](
            execution::Handler<void(inline_executor)> h,
            const inline_executor& ex
          )
        {
          std::invoke(h, ex);
        },
        std::forward<R>(r),
        *this
      );
  }
};

//------------------------------------------------------------------------------

inline constexpr struct use_future_t {} use_future;

namespace execution {

template <class R, class ResultArg>
struct receiver_traits< ::use_future_t, R(ResultArg)>
{
  using receiver_type = use_future_t;

  template <class SenderLauncher, class Receiver, class... LaunchArgs>
  static auto connect(SenderLauncher&& sender, Receiver&&, LaunchArgs&&... launch_args)
  {
    std::promise<std::decay_t<ResultArg>> promise;
    auto future = promise.get_future();
    sender(
        [promise = std::move(promise)](ResultArg result) mutable
        {
          promise.set_value(std::move(result));
        },
        std::forward<LaunchArgs>(launch_args)...
      );
    return future;
  }
};

} // namespace execution

//------------------------------------------------------------------------------

inline constexpr struct block_this_thread_t {} block_this_thread;

namespace execution {

template <Signature... Sigs>
struct receiver_traits< ::block_this_thread_t, Sigs...>
{
  using receiver_type = block_this_thread_t;

  template <class SenderLauncher, class Receiver, class... LaunchArgs>
  static auto connect(SenderLauncher&& sender, Receiver&&, LaunchArgs&&... launch_args)
  {
    return execution::connect<Sigs...>(
        std::forward<SenderLauncher>(sender),
        use_future,
        std::forward<LaunchArgs>(launch_args)...
      ).get();
  }
};

} // namespace execution

//------------------------------------------------------------------------------

int main()
{
  inline_executor ex1;
  ex1.execute([](auto){ std::printf("hello 1\n"); });
  static_assert(execution::Executor<inline_executor>);
  static_assert(propria::query(ex1, execution::blocking) == execution::blocking.possibly);

  auto fut1 = ex1.execute(use_future);
  inline_executor sub_ex1 = fut1.get();
  (void)sub_ex1;

  inline_executor sub_ex2 = ex1.execute(block_this_thread);
  (void)sub_ex2;

  using executor_t = execution::executor<execution::blocking_t>;
    
  executor_t ex2 = ex1;
  ex2.execute([](auto){ std::printf("hello 2\n"); });
  static_assert(execution::Executor<executor_t>);
  assert(propria::query(ex2, execution::blocking) == execution::blocking.possibly);

  auto fut2 = ex2.execute(use_future);
  execution::executor<> sub_ex3 = fut2.get();
  (void)sub_ex3;

  execution::executor<> sub_ex4 = ex2.execute(block_this_thread);
  (void)sub_ex4;
}
