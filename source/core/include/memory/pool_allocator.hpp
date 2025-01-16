/**************************************************************************/
/* pool-allocator.hpp                                                     */
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

#include "memory/heap_allocator.hpp"
#include <vector>

namespace kn {
template<typename T> class PoolAllocator
{
  PoolAllocator(const PoolAllocator &) = delete;
  PoolAllocator &operator=(const PoolAllocator &) = delete;

public:
  PoolAllocator(size_t blockCount)
    : _blockSize(sizeof(T)), _blockCount(blockCount), _pool(nullptr), _freeBlocks(blockCount)
  {
    uint8_t *poolData = HeapAllocator::get_instance()->allocate<uint8_t>(blockCount);
    _pool = reinterpret_cast<void *>(poolData);
    for (size_t i = 0; i < blockCount; ++i) { _freeBlocks[i] = (T *)((uint8_t *)_pool + i * _blockSize); }
  }

  ~PoolAllocator()
  {
    if (_pool) { free(_pool); }
  }

  T *allocate()
  {
    constexpr size_t alignment = alignof(T);

    if (ensure(!_freeBlocks.empty())) {
      void *ptr = _freeBlocks.back();
      new (ptr) T;
      _freeBlocks.pop_back();
      return ptr;
    }

    return nullptr;
  }

  void deallocate(T *ptr)
  {
    delete ptr;
    _freeBlocks.push_back(ptr);
  }

private:
  size_t _blockSize;
  size_t _blockCount;
  T *_pool;
  std::vector<void *> _freeBlocks;
};
}// namespace kn
