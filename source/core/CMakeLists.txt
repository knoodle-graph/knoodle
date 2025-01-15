knoodle_create_module(
  NAME core
  NAMESPACE knoodle
  ENABLE_PCH On)

target_sources(core
  PRIVATE
    src/core.cpp

  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS include
  FILES
    include/math/kn-math.hpp)

knoodle_create_tests(
  NAME core-test
  DEPENDS core)

target_sources(core-test
  PRIVATE
    tests/math/kn-math-test.cpp)