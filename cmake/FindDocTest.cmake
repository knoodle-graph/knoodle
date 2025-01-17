find_path(doctest_INCLUDE_DIR NAMES "doctest.h" "doctest/doctest.h"
  HINTS
    "${PROJECT_SOURCE_DIR}/third_party/doctest/include"
  REQUIRED)

mark_as_advanced(doctest_INCLUDE_DIR)

add_library(doctest INTERFACE)
target_include_directories(doctest
  INTERFACE
    ${doctest_INCLUDE_DIR})
