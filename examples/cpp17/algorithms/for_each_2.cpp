#include "for_each_2.hpp"
#include <iostream>
#include <list>
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

  template <class T>
  accel_executor<algorithm::extensions::for_each_2_t<T*>> require_concept(
      algorithm::extensions::for_each_2_t<T*>) const noexcept
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
  void for_each_2(ForwardIt first, ForwardIt last, UnaryFunction2 f) const
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

  template <class ForwardIt>
  static constexpr bool query(algorithm::extensions::for_each_2_t<ForwardIt>)
  {
    return true;
  }

  template <class ForwardIt, class UnaryFunction2>
  void for_each_2(ForwardIt first, ForwardIt last, UnaryFunction2 f) const
  {
    std::cout << "Starting third party for_each\n";
    for (ForwardIt iter = first; iter != last; ++iter)
      f(*iter);
    std::cout << "Ending third party for_each\n";
  }
};

template <class ForwardIt>
third_party_adapter require_concept(
    const third_party_executor& ex,
    algorithm::extensions::for_each_2_t<ForwardIt>)
{
  return {ex};
}

//------------------------------------------------------------------------------

int main()
{
  std::vector<int> vec = { 1, 2, 3 };
  std::list<int> lst = { 1, 2, 3 };

  inline_executor ex1;
  accel_executor<> ex2;
  third_party_executor ex3;

  algorithm::for_each_2(ex1,
      vec.data(), vec.data() + vec.size(),
      [](auto x){ std::cout << x << "\n"; });

  algorithm::for_each_2(ex2,
      vec.data(), vec.data() + vec.size(),
      [](auto x){ std::cout << x << "\n"; });

  algorithm::for_each_2(ex3,
      vec.data(), vec.data() + vec.size(),
      [](auto x){ std::cout << x << "\n"; });

  algorithm::for_each_2(ex1,
      lst.begin(), lst.end(),
      [](auto x){ std::cout << x << "\n"; });

  algorithm::for_each_2(ex2,
      lst.begin(), lst.end(),
      [](auto x){ std::cout << x << "\n"; });

  algorithm::for_each_2(ex3,
      lst.begin(), lst.end(),
      [](auto x){ std::cout << x << "\n"; });
}
