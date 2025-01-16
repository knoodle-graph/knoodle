knoodle_create_module(
  NAME core
  NAMESPACE knoodle
  ENABLE_PCH On)

target_sources(core
  PRIVATE
    "memory/buddy_allocator.cpp"
    "memory/heap_allocator.cpp"
    "memory/pool_allocator.cpp"
    "memory/stack_allocator.cpp"

  PUBLIC
    "${KNOODLE_ROOT_DIR}/include/core/kn_assert.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/math/kn_math.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/memory/buddy_allocator.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/memory/heap_allocator.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/memory/pool_allocator.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/memory/stack_allocator.hpp"
)

# knoodle_add_tests(NAME "TestMathOperations" COMMAND "math_ops_test" FILE "tests/math/kn_math_test.cpp" DEPENDS core)
# knoodle_add_tests(NAME "TestMemoryAllocators" COMMAND "memory_allocators_test" FILE "tests/memory/allocator_test.cpp" DEPENDS core)

