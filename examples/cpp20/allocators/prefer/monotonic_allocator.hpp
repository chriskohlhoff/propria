//
// monotonic_allocator.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MONOTONIC_ALLOCATOR_HPP
#define MONOTONIC_ALLOCATOR_HPP

#include <cstddef>
#include <memory>
#include "allocator.hpp"

class monotonic_allocator;

class monotonic_resource
{
public:
  monotonic_resource(std::size_t size)
    : buffer_(::operator new(size)),
      ptr_(buffer_),
      size_(size)
  {
  }

  monotonic_resource(const monotonic_resource&) = delete;
  monotonic_resource& operator=(const monotonic_resource&) = delete;

  ~monotonic_resource()
  {
    ::operator delete(buffer_);
  }

  monotonic_allocator get_allocator() noexcept;

private:
  friend class monotonic_allocator;

  void* buffer_;
  void* ptr_;
  std::size_t size_;
};

class monotonic_allocator
{
public:
  monotonic_allocator(monotonic_resource& resource, std::size_t align = 0)
    : resource_(resource),
      alignment_(align)
  {
  }

  monotonic_allocator(const monotonic_allocator&) noexcept = default;
  monotonic_allocator& operator=(const monotonic_allocator&) noexcept = delete;

  void* allocate(std::size_t n) const
  {
    if (std::align(alignment_ ? alignment_ : 1, n, resource_.ptr_, resource_.size_))
    {
      void* result = resource_.ptr_;
      resource_.ptr_ = static_cast<unsigned char*>(resource_.ptr_) + n;
      resource_.size_ -= n;
      return result;
    }
    else
      throw std::bad_alloc();
  }

  void deallocate(void*) const
  {
  }

  std::size_t query(alignment) const noexcept
  {
    return alignment_;
  }

  monotonic_allocator require(alignment a) const
  {
    return {resource_, a.requested_alignment_};
  }

private:
  monotonic_resource& resource_;
  std::size_t alignment_;
};

inline monotonic_allocator monotonic_resource::get_allocator() noexcept
{
  return monotonic_allocator(*this);
}

static_assert(is_allocator_v<monotonic_allocator>);

#endif // MONOTONIC_ALLOCATOR_HPP
