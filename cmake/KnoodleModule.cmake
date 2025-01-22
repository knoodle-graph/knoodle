function(knoodle_setup_module)
cmake_policy(SET CMP0103 NEW)

  set(_ONE_VALUE_ARGS
    TARGET
    API)

  set(_MULTI_VALUE_ARGS
    PUBLIC_DEPENDS
    PRIVATE_DEPENDS
  )

  set(_OPTION_ARGS)

  cmake_parse_arguments(KN_MOD
    "${_OPTION_ARGS}"
    "${_ONE_VALUE_ARGS}"
    "${_MULTI_VALUE_ARGS}"
    ${ARGN})

  set_target_properties(${KN_MOD_TARGET} PROPERTIES
    FOLDER "${KN_MOD_TARGET}"
    POSITION_INDEPENDENT_CODE ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

  target_include_directories(${KN_MOD_TARGET}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include/${KN_MOD_TARGET}>

    PRIVATE
      $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>)

  target_compile_features(${KN_MOD_TARGET} INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
  target_compile_options(${KN_MOD_TARGET}
    PRIVATE
      $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /wd4251 /wd4275>
      $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>)

  target_link_libraries(${KN_MOD_TARGET}
    PUBLIC
      ${KN_MOD_PUBLIC_DEPENDS}
    PRIVATE
      ${KN_MOD_PRIVATE_DEPENDS})      

  set(MODULE_API_NAME)
  if(KN_MOD_API)
    set(MODULE_API_NAME ${KN_MOD_API})
   else()
    set(MODULE_API_NAME "KN_${KN_MOD_TARGET}_API")
  endif()
  string(TOUPPER ${MODULE_API_NAME} MODULE_API_NAME)

  include (GenerateExportHeader)
  generate_export_header(${KN_MOD_TARGET} EXPORT_MACRO_NAME ${MODULE_API_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/include/${KN_MOD_TARGET}_api.hpp")

if(KNOODLE_BUILD_INSTALL)
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${KN_MOD_TARGET}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${KN_MOD_TARGET}-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${KN_MOD_TARGET}-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${KN_MOD_TARGET}")

  include(GNUInstallDirs)
  install(TARGETS ${KN_MOD_TARGET}
        EXPORT ${KN_MOD_TARGET}-targets
        RUNTIME
          COMPONENT Runtime
          DESTINATION bin
        LIBRARY
          COMPONENT Runtime
          DESTINATION lib
          NAMELINK_COMPONENT Development
        ARCHIVE
          COMPONENT Development
          DESTINATION lib/static
        FILE_SET HEADERS
          COMPONENT Development)

  install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" FILES_MATCHING PATTERN "*.hpp")
  install(FILES "${CMAKE_BINARY_DIR}/include/${KN_MOD_TARGET}_api.hpp" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
endif()

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

    target_include_directories(${KNOODLE_TESTS_COMMAND}
      PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/${KNOODLE_TESTS_COMMAND}>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>)

    target_link_libraries(${KNOODLE_TESTS_COMMAND}
      PRIVATE
      doctest::doctest
      ${KNOODLE_TESTS_DEPENDS})

    add_test(NAME ${KNOODLE_TESTS_NAME} COMMAND ${KNOODLE_TESTS_COMMAND})
  endif()
endfunction()
