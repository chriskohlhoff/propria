#include "execution.hpp"
#include <cstdio>
#include <cassert>

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

  template <class F>
  void execute(F f) const noexcept
  {
    f();
  }
};

//------------------------------------------------------------------------------

int main()
{
  inline_executor ex1;
  ex1.execute([]{ std::printf("hello 1\n"); });
  static_assert(execution::Executor<inline_executor>);
  static_assert(propria::query(ex1, execution::blocking) == execution::blocking.possibly);

  using executor_t = execution::executor<execution::blocking_t>;
    
  executor_t ex2 = ex1;
  ex2.execute([]{ std::printf("hello 2\n"); });
  static_assert(execution::Executor<executor_t>);
  assert(propria::query(ex2, execution::blocking) == execution::blocking.possibly);
}
