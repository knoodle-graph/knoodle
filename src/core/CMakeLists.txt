knoodle_create_module(
  NAME core
  NAMESPACE knoodle)

target_sources(core
  PRIVATE
    
    "config/config_manager.cpp"
    "log/log.cpp"
    "memory/stack_allocator.cpp"
    "memory/stack_allocator.cpp"
    "$<$<PLATFORM_ID:Windows>:os/os_windows.cpp>"

  PUBLIC
    "${KNOODLE_ROOT_DIR}/include/core/config/config_manager.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/kn_assert.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/log/log.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/math/kn_math.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/memory/heap_allocator.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/memory/pool_allocator.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/memory/stack_allocator.hpp"
    "${KNOODLE_ROOT_DIR}/include/core/os/os.hpp"
)

knoodle_add_tests(NAME "TestMathOperations" COMMAND "math_ops_test" FILE "${KNOODLE_ROOT_DIR}/tests/core/math/test_math_ops.cpp" DEPENDS core)
knoodle_add_tests(NAME "TestConfigSystem" COMMAND "config_test" FILE "${KNOODLE_ROOT_DIR}/tests/core/config/test_config.cpp" DEPENDS core)
knoodle_add_tests(NAME "TestStringUtils " COMMAND "string_utils_test" FILE "${KNOODLE_ROOT_DIR}/tests/core/test_string_utils.cpp" DEPENDS core)

add_custom_command(TARGET core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${KNOODLE_ROOT_DIR}/tests/core/res
    ${CMAKE_CURRENT_BINARY_DIR}/res)
