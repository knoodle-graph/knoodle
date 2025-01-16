/**************************************************************************/
/* allocator-test.cpp                                                     */
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "memory/heap-allocator.hpp"

TEST_CASE("HeapAllocator::get_allocated_size")
{
  auto allocator = kn::HeapAllocator::get_instance();
  CHECK(allocator->get_allocated_size() == 0);
}

TEST_CASE("HeapAllocator::get_deallocated_size")
{
  auto allocator = kn::HeapAllocator::get_instance();
  CHECK(allocator->get_deallocated_size() == 0);
}

TEST_CASE("HeapAllocator::get_total_size")
{
  auto allocator = kn::HeapAllocator::get_instance();
  CHECK(allocator->get_total_size() == 0);
}

TEST_CASE("HeapAllocator::allocate and deallocate")
{
  auto allocator = kn::HeapAllocator::get_instance();

  SUBCASE("Allocate and deallocate single int")
  {
    int *ptr = allocator->allocate<int>();
    REQUIRE(ptr != nullptr);
    CHECK(allocator->get_allocated_size() == sizeof(int));
    CHECK(allocator->get_total_size() == sizeof(int));

    allocator->deallocate(ptr);
    CHECK(allocator->get_deallocated_size() == sizeof(int));
    CHECK(allocator->get_total_size() == 0);
  }

  SUBCASE("Allocate and deallocate array of ints")
  {
    const size_t count = 10;
    int *ptr = allocator->allocate<int>(count);
    REQUIRE(ptr != nullptr);

    allocator->deallocate(ptr, count);
    CHECK(allocator->get_total_size() == 0);
  }
}

TEST_CASE("HeapAllocator::get_instance")
{
  auto allocator1 = kn::HeapAllocator::get_instance();
  auto allocator2 = kn::HeapAllocator::get_instance();
  CHECK(allocator1 == allocator2);
}

TEST_CASE("HeapAllocator zero leaks")
{
  auto allocator = kn::HeapAllocator::get_instance();
  CHECK(allocator->get_total_size() == 0);
}
