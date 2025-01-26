add_library(core ${LIB_TYPE})
add_library(knoodle::core ALIAS core)

knoodle_setup_module(
  TARGET core
  PUBLIC_DEPENDS yaml-cpp::yaml-cpp)

target_sources(core
  PRIVATE    
    "${CMAKE_CURRENT_SOURCE_DIR}/config/config_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/log/log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/heap_allocator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/memory/stack_allocator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/$<$<PLATFORM_ID:Windows>:os/os_windows.cpp>"
    "${CMAKE_CURRENT_SOURCE_DIR}/$<$<PLATFORM_ID:Linux>:os/os_linux.cpp>"
    "${CMAKE_CURRENT_SOURCE_DIR}/$<$<PLATFORM_ID:Darwin>:os/os_linux.cpp>"

  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
  FILES
    "config/config_manager.hpp"
    "kn_assert.hpp"
    "log/log.hpp"
    "math/kn_math.hpp"
    "memory/heap_allocator.hpp"
    "memory/pool_allocator.hpp"
    "memory/stack_allocator.hpp"
    "memory/smart_ptr.hpp"
    "os/os.hpp"
)

# Thanks gcc for being stuck in the past as your fans.
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        8303d140a1a11f19b982a9f664bbe59a1ccda3f4) # 11.1.2

set(FMT_SYSTEM_HEADERS ON CACHE INTERNAL "")
FetchContent_MakeAvailable(fmt)

target_link_libraries(core PUBLIC fmt::fmt)

knoodle_add_tests(NAME "TestMathOperations" COMMAND "math_ops_test" FILE "${KNOODLE_ROOT_DIR}/tests/core/math/test_math_ops.cpp" DEPENDS core)
knoodle_add_tests(NAME "TestConfigSystem" COMMAND "config_test" FILE "${KNOODLE_ROOT_DIR}/tests/core/config/test_config.cpp" DEPENDS core)
knoodle_add_tests(NAME "TestStringUtils " COMMAND "string_utils_test" FILE "${KNOODLE_ROOT_DIR}/tests/core/test_string_utils.cpp" DEPENDS core)

if(BUILD_TESTING)
  add_custom_command(TARGET core POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${KNOODLE_ROOT_DIR}/tests/core/res
      ${CMAKE_CURRENT_BINARY_DIR}/res)
endif()
