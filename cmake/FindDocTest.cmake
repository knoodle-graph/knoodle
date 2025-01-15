# FindDocTest.cmake

# Locate doctest.h
find_path(DOCTEST_INCLUDE_DIR doctest.h
    PATHS ${PROJECT_SOURCE_DIR}/third_party/doctest/include/doctest
    NO_DEFAULT_PATH
)

# Handle the case where doctest.h is not found
if (NOT DOCTEST_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find doctest.h in third_party/doctest")
endif()

# Provide the include directory to the caller
set(DOCTEST_INCLUDE_DIRS ${DOCTEST_INCLUDE_DIR})

# Mark the cache variables as advanced
mark_as_advanced(DOCTEST_INCLUDE_DIR)