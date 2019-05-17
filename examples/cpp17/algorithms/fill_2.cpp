#include "fill_2.hpp"
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

  template <class T>
  static constexpr bool query(algorithm::extensions::fill_2_t<T*>)
  {
    return true;
  }

  template <class ForwardIt, class T>
  void fill_2(ForwardIt first, ForwardIt last, const T& value) const
  {
    std::cout << "Starting accelerated fill\n";
    for (ForwardIt iter = first; iter != last; ++iter)
      *iter = value;
    std::cout << "Ending accelerated fill\n";
  }
};

static_assert(execution::oneway_t::static_query_v<accel_executor>);
static_assert(algorithm::extensions::fill_2_t<int*>::static_query_v<accel_executor>);

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
  static constexpr bool query(algorithm::extensions::fill_2_t<ForwardIt>)
  {
    return true;
  }

  template <class ForwardIt, class T>
  void fill_2(ForwardIt first, ForwardIt last, const T& value) const
  {
    std::cout << "Starting third party fill\n";
    for (ForwardIt iter = first; iter != last; ++iter)
      *iter = value;
    std::cout << "Ending third party fill\n";
  }
};

template <class ForwardIt>
third_party_adapter require_concept(
    const third_party_executor& ex,
    algorithm::extensions::fill_2_t<ForwardIt>)
{
  return {ex};
}

static_assert(execution::oneway_t::static_query_v<third_party_executor>);
static_assert(algorithm::extensions::fill_2_t<int*>::static_query_v<third_party_adapter>);

//------------------------------------------------------------------------------

int main()
{
  std::vector<int> vec = { 1, 2, 3 };
  std::list<int> lst = { 1, 2, 3 };

  inline_executor ex1;
  accel_executor ex2;
  third_party_executor ex3;

  algorithm::fill_2(ex1, vec.data(), vec.data() + vec.size(), 111);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_2(ex2, vec.data(), vec.data() + vec.size(), 222);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_2(ex3, vec.data(), vec.data() + vec.size(), 333);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_2(ex1, lst.begin(), lst.end(), 444);
  for (const auto& value : lst)
    std::cout << value << "\n";

  algorithm::fill_2(ex2, lst.begin(), lst.end(), 555);
  for (const auto& value : lst)
    std::cout << value << "\n";

  algorithm::fill_2(ex3, lst.begin(), lst.end(), 666);
  for (const auto& value : lst)
    std::cout << value << "\n";
}
