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

template <class T>
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

  monotonic_allocator<char> get_allocator() noexcept;

private:
  template <class T>
  friend class monotonic_allocator;

  void* buffer_;
  void* ptr_;
  std::size_t size_;
};

template <class T>
class monotonic_allocator
{
public:
  monotonic_allocator(monotonic_resource& resource)
    : resource_(resource)
  {
  }

  monotonic_allocator(const monotonic_allocator&) noexcept = default;
  monotonic_allocator& operator=(const monotonic_allocator&) noexcept = delete;

  T* allocate(std::size_t n) const
  {
    if (std::align(alignof(T), n * sizeof(T), resource_.ptr_, resource_.size_))
    {
      T* result = static_cast<T*>(resource_.ptr_);
      resource_.ptr_ = static_cast<unsigned char*>(resource_.ptr_) + n * sizeof(T);
      resource_.size_ -= n * sizeof(T);
      return result;
    }
    else
      throw std::bad_alloc();
  }

  void deallocate(T*) const
  {
  }

  template <class U>
  monotonic_allocator<U> require_concept(allocator_of<U>) const
  {
    return monotonic_allocator<U>(resource_);
  }

private:
  monotonic_resource& resource_;
};

inline monotonic_allocator<char> monotonic_resource::get_allocator() noexcept
{
  return monotonic_allocator<char>(*this);
}

static_assert(is_proto_allocator_v<monotonic_allocator<char>>);
static_assert(is_allocator_of_v<monotonic_allocator<char>, char>);

#endif // MONOTONIC_ALLOCATOR_HPP
