/**************************************************************************/
/* buddy-allocator.cpp                                                    */
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

#include "memory/buddy-allocator.hpp"

namespace kn {
BuddyAllocator::BuddyAllocator(size_t size) : _size(align_to_power_of_two(size))
{
  _memory = malloc(_size);
  _free_list.resize(2 * _size - 1, true);
}

BuddyAllocator::~BuddyAllocator() { free(_memory); }

void *BuddyAllocator::allocate(size_t size)
{
  size = align_to_power_of_two(size);
  size_t index = 0;
  size_t level_size = _size;

  while (level_size > size) {
    if (_free_list[get_left_child_index(index)]) {
      index = get_left_child_index(index);
    } else {
      index = get_right_child_index(index);
    }
    level_size /= 2;
  }

  if (!_free_list[index]) { return nullptr; }

  _free_list[index] = false;
  return static_cast<char *>(_memory) + (index - (1ull << (size_t(log2(index + 1ull))))) * size;
}

void BuddyAllocator::deallocate(void *ptr)
{
  size_t offset = static_cast<char *>(ptr) - static_cast<char *>(_memory);
  size_t index = offset / _size + (1ull << ((size_t(log2(offset / _size + 1ull))) - 1ull));

  while (index > 0) {
    _free_list[index] = true;
    size_t buddy_index = get_buddy_index(index);
    if (_free_list[buddy_index]) {
      index = get_parent_index(index);
    } else {
      break;
    }
  }
  _free_list[index] = true;
}
}// namespace kn
