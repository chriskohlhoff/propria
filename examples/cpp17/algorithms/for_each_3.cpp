#include "for_each_3.hpp"
#include <iostream>
#include <vector>

//------------------------------------------------------------------------------

struct inline_executor
{
  static constexpr execution::oneway_t query(execution::executor_concept_t)
  {
    return {};
  }

  template <class F>
  void execute(F f) const noexcept
  {
    f();
  }
};

//------------------------------------------------------------------------------

template <class ExecutorConcept = execution::oneway_t>
struct accel_executor
{
  static constexpr ExecutorConcept query(execution::executor_concept_t)
  {
    return {};
  }

  accel_executor<> require_concept(execution::oneway_t) const noexcept
  {
    return {};
  }

  accel_executor<algorithm::extensions::for_each_3_t> require_concept(
      algorithm::extensions::for_each_3_t) const noexcept
  {
    return {};
  }

  template <class F,
    std::enable_if_t<
      std::is_convertible_v<
        std::conditional_t<true, ExecutorConcept, F>,
        execution::oneway_t
      >
    >* = nullptr>
  void execute(F f) const noexcept
  {
    f();
  }

  template <class ForwardIt, class UnaryFunction2,
    std::enable_if_t<
      !std::is_convertible_v<
        std::conditional_t<true, ExecutorConcept, ForwardIt>,
        execution::oneway_t
      >
    >* = nullptr>
  void for_each_3(ForwardIt first, ForwardIt last, UnaryFunction2 f) const
  {
    std::cout << "Starting accelerated for_each\n";
    for (ForwardIt iter = first; iter != last; ++iter)
      f(*iter);
    std::cout << "Ending accelerated for_each\n";
  }
};

//------------------------------------------------------------------------------

struct third_party_executor
{
  static constexpr execution::oneway_t query(execution::executor_concept_t)
  {
    return {};
  }

  template <class F>
  void execute(F f) const noexcept
  {
    f();
  }
};

struct third_party_adapter
{
  third_party_executor ex;

  static constexpr algorithm::extensions::for_each_3_t query(
      execution::executor_concept_t)
  {
    return {};
  }

  third_party_executor require_concept(execution::oneway_t) const noexcept
  {
    return ex;
  }

  template <class ForwardIt, class UnaryFunction2>
  void for_each_3(ForwardIt first, ForwardIt last, UnaryFunction2 f) const
  {
    std::cout << "Starting third party for_each\n";
    for (ForwardIt iter = first; iter != last; ++iter)
      f(*iter);
    std::cout << "Ending third party for_each\n";
  }
};

third_party_adapter require_concept(
    const third_party_executor& ex,
    algorithm::extensions::for_each_3_t)
{
  return {ex};
}

//------------------------------------------------------------------------------

int main()
{
  std::vector<int> vec = { 1, 2, 3 };

  inline_executor ex1;
  accel_executor<> ex2;
  third_party_executor ex3;

  algorithm::for_each_3(ex1,
      vec.begin(), vec.end(),
      [](auto x){ std::cout << x << "\n"; });

  algorithm::for_each_3(ex2,
      vec.begin(), vec.end(),
      [](auto x){ std::cout << x << "\n"; });

  algorithm::for_each_3(ex3,
      vec.begin(), vec.end(),
      [](auto x){ std::cout << x << "\n"; });
}
