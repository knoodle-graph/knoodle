cmake_minimum_required(VERSION 3.25)

###########################################################
# Global settings                                         #
###########################################################

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

###########################################################
# Project settings                                        #
###########################################################

project(
  "knoodle"
  VERSION 0.0.1
  DESCRIPTION "A tool for creating procedural textures using a node-based graph editor."
  HOMEPAGE_URL "knoodlegraph.org"
  LANGUAGES CXX C)

enable_testing()

###########################################################
# Redirect outputs                                        #
###########################################################
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH} CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)

get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(GENERATOR_IS_MULTI_CONFIG)
  set(TESTS_OUTPUT_DIR ${EXECUTABLE_OUTPUT_PATH}/tests/$<CONFIG>/ CACHE INTERNAL "" FORCE)
else()
  set(TESTS_OUTPUT_DIR ${EXECUTABLE_OUTPUT_PATH}/tests/ CACHE INTERNAL "" FORCE)
endif()

###########################################################
# Global Options                                          #
###########################################################

set(KNOODLE_ROOT_DIR "${CMAKE_SOURCE_DIR}" CACHE INTERNAL "Root directory of the knoodle project")
set(KNOODLE_THIRDPARTY_DIR "${KNOODLE_ROOT_DIR}/third_party" CACHE INTERNAL "Third party directory of the knoodle project")
mark_as_advanced(KNOODLE_ROOT_DIR)
mark_as_advanced(KNOODLE_THIRDPARTY_DIR)

###########################################################
# Dependencies                                            #
###########################################################

include(CTest)
find_package(doctest)

###########################################################
# Project Options                                         #
###########################################################

option(KNOODLE_BUILD_EXAMPLES "Build the examples" ON)
option(KNOODLE_BUILD_DOCS "Build the documentation" OFF)

###########################################################
# Git Versioning                                          #
###########################################################

set(GIT_SHA "unknown" CACHE STRING "Git SHA of the current build")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

###########################################################
# Add target                                              #
###########################################################

set(KNOODLE_MODULE_NAME "knoodle-engine")

add_library(${KNOODLE_MODULE_NAME} SHARED)
target_compile_features(${KNOODLE_MODULE_NAME} INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
target_compile_options(${KNOODLE_MODULE_NAME}
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /wd4251 /wd4275>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic -Werror>)

include (GenerateExportHeader)
generate_export_header(${KNOODLE_MODULE_NAME} EXPORT_MACRO_NAME KN_API EXPORT_FILE_NAME "source/knoodle_engine_api.h")

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_BINARY_DIR}/${KNOODLE_MODULE_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

configure_package_config_file(
  "${CMAKE_SOURCE_DIR}/cmake/${KNOODLE_MODULE_NAME}-config.cmake"
  "${CMAKE_BINARY_DIR}/${KNOODLE_MODULE_NAME}-config.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${KNOODLE_MODULE_NAME}")

configure_file(
  "${CMAKE_SOURCE_DIR}/source/knoodle_version.hpp"
  "${CMAKE_BINARY_DIR}/source/knoodle_version.hpp"
  ESCAPE_QUOTES)

# Set the module properties
target_include_directories(${KNOODLE_MODULE_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/source>

    $<INSTALL_INTERFACE:include>)

include(GNUInstallDirs)
install(TARGETS ${KNOODLE_MODULE_NAME}
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


install(DIRECTORY "${PROJECT_SOURCE_DIR}/source/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" FILES_MATCHING PATTERN "*.hpp")
install(FILES
  "${CMAKE_BINARY_DIR}/source/knoodle_version.hpp"
  "${PROJECT_BINARY_DIR}/source/knoodle_engine_api.h"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

###########################################################
# Add intern subdirectories                               #
###########################################################

add_subdirectory(source)

###########################################################
# Documentation                                           #
###########################################################

if(KNOODLE_BUILD_DOCS)
  find_package(Doxygen REQUIRED)
  if(DOXYGEN_FOUND)
    if(DOXYGEN_OUTPUT_DIRECTORY STREQUAL "")
      set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/docs")
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(CLANG_ASSISTED_PARSING Yes)
    endif()

    configure_file(${CMAKE_SOURCE_DIR}/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)
    doxygen_add_docs(docs
      ${PROJECT_SOURCE_DIR}/source
      CONFIG_FILE ${CMAKE_BINARY_DIR}/Doxyfile
      COMMENT "Generate html pages")
  endif()
endif()

###########################################################
# Installation                                            #
###########################################################

if(CMAKE_SKIP_INSTALL_RULES)
  return()
endif()

install(DIRECTORY
  "${CMAKE_SOURCE_DIR}/config/"
  DESTINATION "config"
  FILES_MATCHING PATTERN "*.ini")

set(CPACK_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

set(CPACK_PACKAGE_VENDOR "knoodle")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")
set(CPACK_GENERATOR "Nuget,ZIP")
set(CPACK_THREADS 0)

include(CPack)
