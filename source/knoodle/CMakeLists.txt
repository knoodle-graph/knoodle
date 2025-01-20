target_sources("knoodle-engine"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/knoodle.cpp"
    
  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
  FILES
    "knoodle.hpp"
)

knoodle_add_tests(NAME "TestKnoodleInit" COMMAND "test_knoodle_init" FILE "${KNOODLE_ROOT_DIR}/tests/knoodle/test_knoodle_init.cpp" DEPENDS "knoodle-engine")
