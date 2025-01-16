/**************************************************************************/
/* stack-allocator.hpp                                                    */
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
class StackAllocator
{
  StackAllocator(const StackAllocator &) = delete;
  StackAllocator &operator=(const StackAllocator &) = delete;

public:
  StackAllocator();
  ~StackAllocator();

  void initialize(size_t size);

  void *allocate(size_t size, size_t alignment = 1)
  {
    if (ensure(reinterpret_cast<size_t>(_current) + size <= reinterpret_cast<size_t>(_end))) {
      size_t ptr = reinterpret_cast<size_t>(_current);
      size_t mod = (alignment - ptr % alignment) % alignment;

      ptr += mod;
      _current = reinterpret_cast<void *>(ptr + size);
      return reinterpret_cast<void *>(ptr);
    }
    return nullptr;
  }

  void deallocate(void *ptr)
  {
    assert(ptr < _current);
    _current = ptr;
  }

  inline size_t get_size() const { return reinterpret_cast<size_t>(_end) - reinterpret_cast<size_t>(_start); }

private:
  void *_start;
  void *_end;
  void *_current;
};
}// namespace kn
