# Create a module for knoodle
function(knoodle_create_module)
  cmake_policy(SET CMP0103 NEW)

  set(_ONE_VALUE_ARGS
    # module name
    NAME
    NAMESPACE)

  set(_MULTI_VALUE_ARGS)

  set(_OPTION_ARGS)

  cmake_parse_arguments(KNOODLE_MODULE
    "${_OPTION_ARGS}"
    "${_ONE_VALUE_ARGS}"
    "${_MULTI_VALUE_ARGS}"
    ${ARGN})

  # Create the module
  add_library(${KNOODLE_MODULE_NAME} SHARED)

  target_compile_features(${KNOODLE_MODULE_NAME} INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

  target_compile_options(${KNOODLE_MODULE_NAME}
    PRIVATE
      $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /wd4251>
      $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

  # Set the module properties
  target_include_directories(${KNOODLE_MODULE_NAME}
    PUBLIC
      $<BUILD_INTERFACE:${KNOODLE_ROOT_DIR}/include/${KNOODLE_MODULE_NAME}>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>

      $<INSTALL_INTERFACE:include>)

  # Set dependencies
  target_link_libraries(${KNOODLE_MODULE_NAME}
    PUBLIC
      ${KNOODLE_MODULE_PUBLIC_DEPENDENCIES}
    PRIVATE
      ${KNOODLE_MODULE_PRIVATE_DEPENDENCIES})
  
  # Setup export headers
  set(MODULE_API_NAME ${KNOODLE_MODULE_NAME}_api)
  string(TOUPPER ${MODULE_API_NAME} MODULE_API_NAME)

  include (GenerateExportHeader)
  generate_export_header(${KNOODLE_MODULE_NAME}
    BASE_NAME "generated/${KNOODLE_MODULE_NAME}"
    PREFIX_NAME KN_
    EXPORT_MACRO_NAME ${MODULE_API_NAME})

  # Setup module installation
  install(TARGETS ${KNOODLE_MODULE_NAME}
        RUNTIME           # Following options apply to runtime artifacts.
          COMPONENT Runtime
        LIBRARY           # Following options apply to library artifacts.
          COMPONENT Runtime
          NAMELINK_COMPONENT Development
        ARCHIVE           # Following options apply to archive artifacts.
          COMPONENT Development
          DESTINATION lib/static
        FILE_SET HEADERS  # Following options apply to file set HEADERS.
          COMPONENT Development)

  install(DIRECTORY "${KNOODLE_ROOT_DIR}/include/${KNOODLE_MODULE_NAME}"
    DESTINATION include)

  install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated/"
    DESTINATION include/${KNOODLE_MODULE_NAME})

  # force include the export header
  target_compile_options(${KNOODLE_MODULE_NAME}
    PUBLIC
      $<$<CXX_COMPILER_ID:MSVC>:/FI${KNOODLE_MODULE_NAME}_export.h>
      $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-include ${KNOODLE_MODULE_NAME}_export.h>) 

  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${KNOODLE_MODULE_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

  configure_package_config_file(
    "${KNOODLE_ROOT_DIR}/cmake/${KNOODLE_MODULE_NAME}-config.cmake.in"
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
      doctest::doctest
      ${KNOODLE_TESTS_DEPENDS})

    add_test(NAME ${KNOODLE_TESTS_NAME} COMMAND ${KNOODLE_TESTS_COMMAND})
  endif()
endfunction()
