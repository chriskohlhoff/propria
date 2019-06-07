#include "execution.hpp"
#include <cassert>
#include <chrono>
#include <iostream>

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

  template <class F>
  void execute(F f) const noexcept
  {
    f(*this);
  }

  unsigned char padding_[Padding];
};

//------------------------------------------------------------------------------

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using float_nanos = std::chrono::duration<float, std::nano>;

const std::size_t iterations = 100'000'000;

int main()
{
  {
    inline_executor<execution::blocking_t::always_t, 1> ex1;
    execution::executor<> ex2(ex1);

    auto start = high_resolution_clock::now();

    std::size_t count = 0;
    for (std::size_t i = 0; i < iterations; ++i)
      ex2.execute([&count](const auto&){ ++count; });
    assert(count == iterations);

    auto end = high_resolution_clock::now();

    std::cout
      << "executor<>, small object, always blocking: "
      << duration_cast<float_nanos>(end - start).count() / iterations
      << " ns per iteration\n";
  }

  {
    inline_executor<execution::blocking_t::always_t, 64> ex1;
    execution::executor<> ex2(ex1);

    auto start = high_resolution_clock::now();

    std::size_t count = 0;
    for (std::size_t i = 0; i < iterations; ++i)
      ex2.execute([&count](const auto&){ ++count; });
    assert(count == iterations);

    auto end = high_resolution_clock::now();

    std::cout
      << "executor<>, large object, always blocking: "
      << duration_cast<float_nanos>(end - start).count() / iterations
      << " ns per iteration\n";
  }

  {
    inline_executor<execution::blocking_t::always_t, 1> ex1;
    execution::executor<execution::occupancy_t, execution::blocking_t::always_t> ex2(ex1);
    execution::executor<execution::occupancy_t> ex3(ex1);

    auto start = high_resolution_clock::now();

    std::size_t count = 0;
    for (std::size_t i = 0; i < iterations; ++i)
      ex3.execute([&count](const auto&){ ++count; });
    assert(count == iterations);

    auto end = high_resolution_clock::now();

    std::cout
      << "narrowed executor<occupancy_t>, small object, always blocking: "
      << duration_cast<float_nanos>(end - start).count() / iterations
      << " ns per iteration\n";
  }

  {
    inline_executor<execution::blocking_t::always_t, 64> ex1;
    execution::executor<execution::occupancy_t, execution::blocking_t::always_t> ex2(ex1);
    execution::executor<execution::occupancy_t> ex3(ex1);

    auto start = high_resolution_clock::now();

    std::size_t count = 0;
    for (std::size_t i = 0; i < iterations; ++i)
      ex3.execute([&count](const auto&){ ++count; });
    assert(count == iterations);

    auto end = high_resolution_clock::now();

    std::cout
      << "narrowed executor<occupancy_t>, large object, always blocking: "
      << duration_cast<float_nanos>(end - start).count() / iterations
      << " ns per iteration\n";
  }

  {
    inline_executor<execution::blocking_t::always_t, 1> ex1;
    execution::executor<execution::occupancy_t, execution::blocking_t::always_t> ex2(ex1);
    execution::executor<> ex3(ex1);

    auto start = high_resolution_clock::now();

    std::size_t count = 0;
    for (std::size_t i = 0; i < iterations; ++i)
      ex3.execute([&count](const auto&){ ++count; });
    assert(count == iterations);

    auto end = high_resolution_clock::now();

    std::cout
      << "narrowed executor<>, small object, always blocking: "
      << duration_cast<float_nanos>(end - start).count() / iterations
      << " ns per iteration\n";
  }

  {
    inline_executor<execution::blocking_t::always_t, 64> ex1;
    execution::executor<execution::occupancy_t, execution::blocking_t::always_t> ex2(ex1);
    execution::executor<> ex3(ex1);

    auto start = high_resolution_clock::now();

    std::size_t count = 0;
    for (std::size_t i = 0; i < iterations; ++i)
      ex3.execute([&count](const auto&){ ++count; });
    assert(count == iterations);

    auto end = high_resolution_clock::now();

    std::cout
      << "narrowed executor<>, large object, always blocking: "
      << duration_cast<float_nanos>(end - start).count() / iterations
      << " ns per iteration\n";
  }

  {
    inline_executor<execution::blocking_t::possibly_t, 1> ex1;
    execution::executor<> ex2(ex1);

    auto start = high_resolution_clock::now();

    std::size_t count = 0;
    for (std::size_t i = 0; i < iterations; ++i)
      ex2.execute([&count](const auto&){ ++count; });
    assert(count == iterations);

    auto end = high_resolution_clock::now();

    std::cout
      << "executor<>, small object, possibly blocking: "
      << duration_cast<float_nanos>(end - start).count() / iterations
      << " ns per iteration\n";
  }

  {
    inline_executor<execution::blocking_t::possibly_t, 64> ex1;
    execution::executor<> ex2(ex1);

    auto start = high_resolution_clock::now();

    std::size_t count = 0;
    for (std::size_t i = 0; i < iterations; ++i)
      ex2.execute([&count](const auto&){ ++count; });
    assert(count == iterations);

    auto end = high_resolution_clock::now();

    std::cout
      << "executor<>, large object, possibly blocking: "
      << duration_cast<float_nanos>(end - start).count() / iterations
      << " ns per iteration\n";
  }

  {
    inline_executor<execution::blocking_t::possibly_t, 1> ex1;
    execution::executor<execution::blocking_t::always_t> ex2(ex1);

    auto start = high_resolution_clock::now();

    std::size_t count = 0;
    for (std::size_t i = 0; i < iterations; ++i)
      propria::require(ex2, execution::blocking.always).execute([&count](const auto&){ ++count; });
    assert(count == iterations);

    auto end = high_resolution_clock::now();

    std::cout
      << "executor<always_t>, small object, always blocking required per iteration: "
      << duration_cast<float_nanos>(end - start).count() / iterations
      << " ns per iteration\n";
  }

  {
    inline_executor<execution::blocking_t::possibly_t, 64> ex1;
    execution::executor<execution::blocking_t::always_t> ex2(ex1);

    auto start = high_resolution_clock::now();

    std::size_t count = 0;
    for (std::size_t i = 0; i < iterations; ++i)
      propria::require(ex2, execution::blocking.always).execute([&count](const auto&){ ++count; });
    assert(count == iterations);

    auto end = high_resolution_clock::now();

    std::cout
      << "executor<always_t>, large object, always blocking required per iteration: "
      << duration_cast<float_nanos>(end - start).count() / iterations
      << " ns per iteration\n";
  }
}
