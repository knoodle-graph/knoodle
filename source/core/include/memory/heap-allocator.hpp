/**************************************************************************/
/* heap-allocator.hpp                                                     */
/**************************************************************************/
/*                         This file is part of:                          */
/*                                Knoodle                                 */
/*                        https://knoodlegraph.org                        */
/**************************************************************************/
/* Copyright (c) 2025 Knoodle contributors (vide AUTHORS.md)              */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include "kn-assert.hpp"

#include <memory>
#include <new>
#include <stdlib.h>

namespace kn {
class HeapAllocator
{
  HeapAllocator(const HeapAllocator &) = delete;
  HeapAllocator &operator=(const HeapAllocator &) = delete;

public:
  HeapAllocator() = default;
  ~HeapAllocator() = default;

  constexpr size_t get_allocated_size() const { return _allocated_size; }
  constexpr size_t get_deallocated_size() const { return _deallocated_size; }
  constexpr size_t get_total_size() const { return _allocated_size - _deallocated_size; }

  static HeapAllocator *get_instance();

  template<typename T> T *allocate(size_t count = 1, size_t alignment = 1)
  {
    size_t desired_size = count * sizeof(T);
    _allocated_size += desired_size;

    T *obj = nullptr;

    void *ptr = nullptr;
#if defined(_MSC_VER)
    ptr = _aligned_malloc(desired_size, alignment);
#else
    posix_memalign(&ptr, alignment, desired_size);
#endif

    if (ensure(ptr)) {
      obj = reinterpret_cast<T *>(ptr);
      for (uint32_t i = 0; i < count; ++i) { new (reinterpret_cast<T *>(ptr) + i) T; }
      return obj;
    }

    return nullptr;
  }

  template<typename T> void deallocate(T *ptr, size_t count = 1)
  {
    if (ensure(ptr)) {
      size_t desired_size = count * sizeof(T);
      _deallocated_size += desired_size;
      for (uint32_t i = 0; i < count; ++i) { ptr[i].~T(); }
    }
  }

private:
  static std::unique_ptr<HeapAllocator> instance;

  size_t _allocated_size = 0;
  size_t _deallocated_size = 0;
};
}// namespace kn
