#include "fill_4.hpp"
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

static_assert(execution::oneway_t::static_query_v<inline_executor>);

//------------------------------------------------------------------------------

struct accel_executor
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

  template <class T, class U>
  static constexpr bool query(algorithm::extensions::fill_4_t<T*, U>)
  {
    return true;
  }

  template <class ForwardIt, class T>
  void fill_4(ForwardIt first, ForwardIt last, const T& value) const
  {
    std::cout << "Starting accelerated fill\n";
    for (ForwardIt iter = first; iter != last; ++iter)
      *iter = value;
    std::cout << "Ending accelerated fill\n";
  }
};

static_assert(execution::oneway_t::static_query_v<accel_executor>);
static_assert(algorithm::extensions::fill_4_t<int*, int>::static_query_v<accel_executor>);

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

  template <class ForwardIt, class T>
  static constexpr bool query(algorithm::extensions::fill_4_t<ForwardIt, T>)
  {
    return true;
  }

  template <class ForwardIt, class T>
  void fill_4(ForwardIt first, ForwardIt last, const T& value) const
  {
    std::cout << "Starting third party fill\n";
    for (ForwardIt iter = first; iter != last; ++iter)
      *iter = value;
    std::cout << "Ending third party fill\n";
  }
};

third_party_adapter require_concept(
    const third_party_executor& ex,
    algorithm::extensions::fill_4_t<int*, int>)
{
  return {ex};
}

static_assert(execution::oneway_t::static_query_v<third_party_executor>);
static_assert(algorithm::extensions::fill_4_t<int*, int>::static_query_v<third_party_adapter>);

//------------------------------------------------------------------------------

int main()
{
  std::vector<int> vec = { 1, 2, 3 };
  std::list<int> lst = { 1, 2, 3 };

  inline_executor ex1;
  accel_executor ex2;
  third_party_executor ex3;

  execution::executor<
      execution::oneway_t,
      algorithm::extensions::fill_4_t<int*, int>
    > ex4 = ex1;

  execution::executor<
      execution::oneway_t,
      algorithm::extensions::fill_4_t<int*, int>
    > ex5 = ex2;

  algorithm::fill_4(ex1, vec.data(), vec.data() + vec.size(), 111);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_4(ex2, vec.data(), vec.data() + vec.size(), 222);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_4(ex3, vec.data(), vec.data() + vec.size(), 333);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_4(ex4, vec.data(), vec.data() + vec.size(), 444);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_4(ex5, vec.data(), vec.data() + vec.size(), 555);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_4(ex1, lst.begin(), lst.end(), 666);
  for (const auto& value : lst)
    std::cout << value << "\n";

  algorithm::fill_4(ex2, lst.begin(), lst.end(), 777);
  for (const auto& value : lst)
    std::cout << value << "\n";
}
