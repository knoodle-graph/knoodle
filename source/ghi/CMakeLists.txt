target_sources("knoodle-engine"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/ghi_manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/null_ghi/null_ghi.cpp"
    
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
  FILES
    "ghi_definitions.hpp"
    "ghi_interface.hpp"
    "ghi_manager.hpp"
    "null_ghi/null_ghi.hpp"
)

option(KNOODLE_BUILD_NULL_GHI "Build the null GHI implementation" ON)

if(KNOODLE_BUILD_NULL_GHI)
  target_compile_definitions("knoodle-engine" PRIVATE KN_BUILD_NULL_GHI=1)
endif()

knoodle_add_tests(NAME "TestGHIManager" COMMAND "test_ghi_manager" FILE "${KNOODLE_ROOT_DIR}/tests/ghi/test_ghi_manager.cpp" DEPENDS "knoodle-engine")
