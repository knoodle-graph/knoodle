knoodle_create_module(
  NAME ghi
  NAMESPACE knoodle
  PRIVATE_DEPENDS core)

target_sources(ghi
  PRIVATE
    "ghi_manager.cpp"
    
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS "${KNOODLE_ROOT_DIR}/include/ghi"
  FILES
    "${KNOODLE_ROOT_DIR}/include/ghi/ghi_definitions.hpp"
    "${KNOODLE_ROOT_DIR}/include/ghi/ghi_interface.hpp"
    "${KNOODLE_ROOT_DIR}/include/ghi/ghi_manager.hpp"
)

option(KNOODLE_BUILD_NULL_GHI "Build the null GHI implementation" ON)

if(KNOODLE_BUILD_NULL_GHI)
  target_compile_definitions(ghi PRIVATE KN_BUILD_NULL_GHI=1)
  target_sources(ghi
    PRIVATE
      "null_ghi/null_ghi.cpp"

    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "${KNOODLE_ROOT_DIR}/include/ghi"
    FILES
      "${KNOODLE_ROOT_DIR}/include/ghi/null_ghi/null_ghi.hpp"
  )
endif()

knoodle_add_tests(NAME "TestGHIManager" COMMAND "test_ghi_manager" FILE "${KNOODLE_ROOT_DIR}/tests/ghi/test_ghi_manager.cpp" DEPENDS core ghi)
