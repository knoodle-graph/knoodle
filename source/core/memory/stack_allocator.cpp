/**************************************************************************/
/* stack_allocator.cpp                                                    */
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

#include "memory/stack_allocator.hpp"
#include <cassert>
#include <cstddef>
#include <cstdlib>

namespace kn {
StackAllocator::~StackAllocator() {
  assert(_start == _current);

  if (_start) {
    free(_start);
  }
}

void StackAllocator::initialize(size_t size) {
  _start = malloc(size);
  _current = _start;
  _end = reinterpret_cast<char*>(reinterpret_cast<std::byte*>(_start) + size);
}
}  // namespace kn
