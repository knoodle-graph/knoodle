knoodle_create_module(
  NAME core
  NAMESPACE knoodle
  ENABLE_PCH On)

target_sources(core
  PRIVATE
    "src/memory/buddy_allocator.cpp"
    "src/memory/heap_allocator.cpp"
    "src/memory/pool_allocator.cpp"
    "src/memory/stack_allocator.cpp"

  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS include
  FILES
    "include/kn_assert.hpp"
    "include/math/kn_math.hpp"
    "include/memory/buddy_allocator.hpp"
    "include/memory/heap_allocator.hpp"
    "include/memory/pool_allocator.hpp"
    "include/memory/stack_allocator.hpp"
)

knoodle_add_tests(NAME "TestMathOperations" COMMAND "math_ops_test" FILE "tests/math/kn_math_test.cpp" DEPENDS core)
knoodle_add_tests(NAME "TestMemoryAllocators" COMMAND "memory_allocators_test" FILE "tests/memory/allocator_test.cpp" DEPENDS core)

