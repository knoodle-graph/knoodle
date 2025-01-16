/**************************************************************************/
/* buddy-allocator.hpp                                                    */
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

#include "memory/heap-allocator.hpp"
#include <cassert>
#include <cmath>
#include <vector>

namespace kn {

class BuddyAllocator
{
  BuddyAllocator(const BuddyAllocator &) = delete;
  BuddyAllocator &operator=(const BuddyAllocator &) = delete;

public:
  BuddyAllocator(size_t size);
  ~BuddyAllocator();

  void *allocate(size_t size);
  void deallocate(void *ptr);

  constexpr size_t get_size() const { return _size; }
  constexpr size_t get_free_size() const { return _free_list.size(); }
  constexpr size_t get_used_size() const { return _size - get_free_size(); }

private:
  constexpr size_t align_to_power_of_two(size_t size) { return size == 0 ? 1 : (1 << (int(log2(size - 1)) + 1)); }
  constexpr size_t get_buddy_index(size_t index) { return index % 2 == 0 ? index - 1 : index + 1; }
  constexpr size_t get_parent_index(size_t index) { return (index - 1) / 2; }
  constexpr size_t get_left_child_index(size_t index) { return 2 * index + 1; }
  constexpr size_t get_right_child_index(size_t index) { return 2 * index + 2; }

  std::vector<bool> _free_list;
  void *_memory;
  size_t _size;
};
}// namespace kn
