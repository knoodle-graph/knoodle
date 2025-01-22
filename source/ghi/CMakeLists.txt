add_library(ghi ${LIB_TYPE})
add_library(knoodle::ghi ALIAS ghi)

knoodle_setup_module(
  TARGET ghi
  PUBLIC_DEPENDS core)

target_sources(ghi
  PRIVATE
    "ghi_manager.cpp"
    "null_ghi/null_ghi.cpp"
    
  PUBLIC
  FILE_SET HEADERS
  FILES
    "ghi_definitions.hpp"
    "ghi_interface.hpp"
    "ghi_manager.hpp"
    "null_ghi/null_ghi.hpp"
)

option(KNOODLE_BUILD_NULL_GHI "Build the null GHI implementation" ON)

if(KNOODLE_BUILD_NULL_GHI)
  target_compile_definitions(ghi PRIVATE KN_BUILD_NULL_GHI=1)
endif()

knoodle_add_tests(NAME "TestGHIManager" COMMAND "test_ghi_manager" FILE "${KNOODLE_ROOT_DIR}/tests/ghi/test_ghi_manager.cpp" DEPENDS ghi)
