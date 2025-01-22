add_library(engine ${LIB_TYPE})
add_library(knoodle::engine ALIAS engine)

knoodle_setup_module(
  TARGET engine
  PRIVATE_DEPENDS core ghi)

target_sources(engine
  PRIVATE
    "engine.cpp"
    
  PUBLIC
  FILE_SET HEADERS
  FILES
    "engine.hpp")

knoodle_add_tests(NAME "TestKnoodleInit" COMMAND "test_knoodle_init" FILE "${KNOODLE_ROOT_DIR}/tests/knoodle/test_knoodle_init.cpp" DEPENDS engine)
