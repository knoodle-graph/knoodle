knoodle_create_module(
  NAME core
  NAMESPACE knoodle
  ENABLE_PCH On)

target_sources(core
  PRIVATE
    
    "memory/heap_allocator.cpp"    
    "memory/stack_allocator.cpp"

  PUBLIC
    "${KNOODLE_ROOT_DIR}/include/core/kn_assert.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/math/kn_math.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/memory/heap_allocator.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/memory/pool_allocator.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/memory/stack_allocator.hpp"
)

knoodle_add_tests(NAME "TestMathOperations" COMMAND "math_ops_test" FILE "${KNOODLE_ROOT_DIR}/tests/core/math/test_math_ops.cpp" DEPENDS core)
# knoodle_add_tests(NAME "TestMemoryAllocators" COMMAND "memory_allocators_test" FILE "tests/memory/allocator_test.cpp" DEPENDS core)

