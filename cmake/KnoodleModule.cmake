# Create a module for knoodle
function(knoodle_create_module)
  cmake_policy(SET CMP0103 NEW)

  set(_ONE_VALUE_ARGS
    # module name
    NAME
    NAMESPACE)

  set(_MULTI_VALUE_ARGS)

  set(_OPTION_ARGS
    ENABLE_PCH)

  cmake_parse_arguments(KNOODLE_MODULE
    "${_OPTION_ARGS}"
    "${_ONE_VALUE_ARGS}"
    "${_MULTI_VALUE_ARGS}"
    ${ARGN})

  # Create the module
  add_library(${KNOODLE_MODULE_NAME} STATIC)

  target_compile_features(${KNOODLE_MODULE_NAME} INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

  target_compile_options(${KNOODLE_MODULE_NAME}
    PRIVATE
      $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
      $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

  # Set the module properties
  target_include_directories(${KNOODLE_MODULE_NAME}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>

      $<INSTALL_INTERFACE:include>)

  # Set dependencies
  target_link_libraries(${KNOODLE_MODULE_NAME}
    PUBLIC
      ${KNOODLE_MODULE_PUBLIC_DEPENDENCIES}
    PRIVATE
      ${KNOODLE_MODULE_PRIVATE_DEPENDENCIES})

  # Setup precompiled headers
  if(KNOODLE_MODULE_ENABLE_PCH)
    target_precompile_headers(${KNOODLE_MODULE_NAME}
      INTERFACE
        <assert.h>
        <concepts>
        <string>
        <utility>
        <vector>)
  endif()

  # Setup export headers
  set(MODULE_API_NAME ${KNOODLE_MODULE_NAME}_api)
  string(TOUPPER ${MODULE_API_NAME} MODULE_API_NAME)

  include (GenerateExportHeader)
  generate_export_header(${KNOODLE_MODULE_NAME}
    BASE_NAME include/${KNOODLE_MODULE_NAME}
    PREFIX_NAME KN_
    EXPORT_MACRO_NAME ${MODULE_API_NAME})

  # Setup module installation
  install(TARGETS ${KNOODLE_MODULE_NAME}
    EXPORT ${KNOODLE_MODULE_NAME}Targets
    LIBRARY DESTINATION shlib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${KNOODLE_MODULE_NAME}" COMPONENT dev)

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${KNOODLE_MODULE_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${KNOODLE_MODULE_NAME}-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${KNOODLE_MODULE_NAME}-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${KNOODLE_MODULE_NAME}")

endfunction()

function(knoodle_add_tests)
  cmake_policy(SET CMP0103 NEW)

  if(BUILD_TESTING)
    set(_ONE_VALUE_ARGS
      # module name
      NAME
      COMMAND
      FILE)

    set(_MULTI_VALUE_ARGS
      # module dependencies
      DEPENDS)

    set(_OPTION_ARGS)

    cmake_parse_arguments(KNOODLE_TESTS
      "${_OPTION_ARGS}"
      "${_ONE_VALUE_ARGS}"
      "${_MULTI_VALUE_ARGS}"
      ${ARGN})

    add_executable(${KNOODLE_TESTS_COMMAND} ${KNOODLE_TESTS_FILE})
    target_compile_features(${KNOODLE_TESTS_COMMAND} INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

    target_link_libraries(${KNOODLE_TESTS_COMMAND}
      PRIVATE
      doctest
      ${KNOODLE_TESTS_DEPENDS})

    add_test(NAME ${KNOODLE_TESTS_NAME} COMMAND ${KNOODLE_TESTS_COMMAND})
  endif()
endfunction()
