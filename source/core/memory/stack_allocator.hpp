/**************************************************************************/
/* stack_allocator.hpp                                                    */
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

#include <stdlib.h>
#include <cstddef>
#include <memory>

#include "common.hpp"
#include "kn_assert.hpp"

namespace kn {
class KN_CORE_API StackAllocator {
 public:
  StackAllocator() = default;
  StackAllocator(const StackAllocator&) = delete;
  StackAllocator& operator=(const StackAllocator&) = delete;

  ~StackAllocator();

  void initialize(size_t size);

  void* allocate(std::size_t size, std::size_t alignment = alignof(std::max_align_t)) {
    std::size_t space = static_cast<char*>(_end) - static_cast<char*>(_current);
    void* alignedPtr = _current;

    if (std::align(alignment, size, alignedPtr, space)) {
      _current = static_cast<char*>(alignedPtr) + size;
      return alignedPtr;
    }

    return nullptr;
  }

  void deallocate(void* ptr) {
    assert(ptr < _current);
    _current = ptr;
  }

  [[nodiscard]] inline size_t get_size() const {
    return reinterpret_cast<uintptr_t>(_end) - reinterpret_cast<uintptr_t>(_start);
  }

 private:
  void* _start{nullptr};
  void* _end{nullptr};
  void* _current{nullptr};
};
}  // namespace kn
