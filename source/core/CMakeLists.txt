knoodle_create_module(
  NAME core
  NAMESPACE knoodle
  ENABLE_PCH On)

target_sources(core
  PRIVATE
    "src/memory/buddy-allocator.cpp"
    "src/memory/heap-allocator.cpp"
    "src/memory/pool-allocator.cpp"
    "src/memory/stack-allocator.cpp"

  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS include
  FILES
    "include/kn-assert.hpp"
    "include/math/kn-math.hpp"
    "include/memory/buddy-allocator.hpp"
    "include/memory/heap-allocator.hpp"
    "include/memory/pool-allocator.hpp"
    "include/memory/stack-allocator.hpp"
)

knoodle_create_tests(
  NAME core_test
  DEPENDS core)

target_sources(core_test
  PRIVATE
    "tests/math/kn-math-test.cpp"
    "tests/memory/allocator-test.cpp")
