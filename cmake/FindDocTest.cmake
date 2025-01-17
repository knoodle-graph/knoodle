find_path(DOCTEST_INCLUDE_DIR doctest/doctest.h
    HINTS ${KNOODLE_THIRDPARTY_DIR}/doctest/include)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(doctest DEFAULT_MSG DOCTEST_INCLUDE_DIR)

# Add imported target.
if(DOCTEST_FOUND)
    set(DOCTEST_INCLUDE_DIRS "${DOCTEST_INCLUDE_DIR}")

    if(NOT TARGET doctest::doctest)
        add_library(doctest::doctest INTERFACE IMPORTED)
        set_target_properties(doctest::doctest PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${DOCTEST_INCLUDE_DIRS}")
    endif()
endif()
