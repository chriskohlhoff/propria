#include "fill_1.hpp"
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

  static constexpr std::size_t query(execution::occupancy_t)
  {
    return 42;
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

  static constexpr bool query(algorithm::extensions::fill_1_t)
  {
    return true;
  }

  template <class ForwardIt, class T>
  void fill_1(ForwardIt first, ForwardIt last, const T& value) const
  {
    std::cout << "Starting accelerated fill\n";
    for (ForwardIt iter = first; iter != last; ++iter)
      *iter = value;
    std::cout << "Ending accelerated fill\n";
  }
};

static_assert(execution::oneway_t::static_query_v<accel_executor>);
static_assert(algorithm::extensions::fill_1_t::static_query_v<accel_executor>);

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

  static constexpr bool query(algorithm::extensions::fill_1_t)
  {
    return true;
  }

  template <class ForwardIt, class T>
  void fill_1(ForwardIt first, ForwardIt last, const T& value) const
  {
    std::cout << "Starting third party fill\n";
    for (ForwardIt iter = first; iter != last; ++iter)
      *iter = value;
    std::cout << "Ending third party fill\n";
  }
};

third_party_adapter require_concept(
    const third_party_executor& ex,
    algorithm::extensions::fill_1_t)
{
  return {ex};
}

static_assert(execution::oneway_t::static_query_v<third_party_executor>);
static_assert(algorithm::extensions::fill_1_t::static_query_v<third_party_adapter>);

//------------------------------------------------------------------------------

int main()
{
  std::vector<int> vec = { 1, 2, 3 };

  inline_executor ex1;
  accel_executor ex2;
  third_party_executor ex3;

  execution::executor<execution::oneway_t, execution::occupancy_t> ex4 = ex1;
  std::cout << "occupancy " << propria::query(ex4, execution::occupancy) << "\n";

  algorithm::fill_1(ex1, vec.begin(), vec.end(), 111);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_1(ex2, vec.begin(), vec.end(), 222);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_1(ex3, vec.begin(), vec.end(), 333);
  for (const auto& value : vec)
    std::cout << value << "\n";

  algorithm::fill_1(ex4, vec.begin(), vec.end(), 444);
  for (const auto& value : vec)
    std::cout << value << "\n";
}
