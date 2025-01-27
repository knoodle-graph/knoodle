cmake_minimum_required(VERSION 3.25)

###########################################################
# Global settings                                         #
###########################################################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are:
        None Debug Release RelWithDebInfo Profile."
      FORCE)
endif()

###########################################################
# Project settings                                        #
###########################################################

project(
  "knoodle"
  VERSION 0.0.1
  DESCRIPTION "A tool for creating procedural textures using a node-based graph editor."
  HOMEPAGE_URL "knoodlegraph.org"
  LANGUAGES C CXX)

enable_testing()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

###########################################################
# Redirect outputs                                        #
###########################################################

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_PROFILE "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_PROFILE "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_PROFILE "${CMAKE_BINARY_DIR}/bin")

###########################################################
# Global Options                                          #
###########################################################

set(KNOODLE_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Root directory of the knoodle project")
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

option(BUILD_SHARED_LIBS "Build Shared Libraries" ON)
option(KNOODLE_BUILD_EXAMPLES "Build the examples" ON)
option(KNOODLE_BUILD_DOCS "Build the documentation" OFF)
option(KNOODLE_BUILD_INSTALL "Build the installation" ON)
option(KNOODLE_BUILD_TOOLS "Build the tools" ON)
option(KNOODLE_WITH_VULKAN "Build with Vulkan support" ON)

set(LIB_TYPE STATIC)

if(BUILD_SHARED_LIBS)
    set(LIB_TYPE SHARED)
endif()

###########################################################
# Git Versioning                                          #
###########################################################

set(GIT_SHA "unknown" CACHE STRING "Git SHA of the current build")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

###########################################################
# Add target                                              #
###########################################################

include(KnoodleModule)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/source/knoodle_version.hpp"
  "${CMAKE_CURRENT_BINARY_DIR}/source/knoodle_version.hpp"
  ESCAPE_QUOTES)

###########################################################
# Add extern subdirectories                               #
###########################################################

function(target_link_libraries_system target)
  set(options PRIVATE PUBLIC INTERFACE)
  cmake_parse_arguments(TLLS "${options}" "" "" ${ARGN})
  foreach(op ${options})
    if(TLLS_${op})
      set(scope ${op})
    endif()
  endforeach(op)
  set(libs ${TLLS_UNPARSED_ARGUMENTS})

  foreach(lib ${libs})
    get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)
    if(lib_include_dirs)
      if(scope)
        target_include_directories(${target} SYSTEM ${scope} ${lib_include_dirs})
      else()
        target_include_directories(${target} SYSTEM PRIVATE ${lib_include_dirs})
      endif()
    else()
      message("Warning: ${lib} doesn't set INTERFACE_INCLUDE_DIRECTORIES. No include_directories set.")
    endif()
    if(scope)
      target_link_libraries(${target} ${scope} ${lib})
    else()
      target_link_libraries(${target} ${lib})
    endif()
  endforeach()
endfunction(target_link_libraries_system)

set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "")
set(YAML_BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
add_subdirectory(third_party/yaml-cpp)
target_link_libraries_system(yaml-cpp)
target_compile_options(yaml-cpp
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/wd4267>)

###########################################################
# Add intern subdirectories                               #
###########################################################

add_subdirectory(source)

if(KNOODLE_BUILD_TOOLS)
  add_subdirectory(tools)
endif()

###########################################################
# Documentation                                           #
###########################################################

if(KNOODLE_BUILD_DOCS)
  find_package(Doxygen REQUIRED)
  if(DOXYGEN_FOUND)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(CLANG_ASSISTED_PARSING Yes)
    endif()

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    doxygen_add_docs(docs
      ${PROJECT_SOURCE_DIR}/source
      CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      COMMENT "Generate html pages")
  endif()
endif()

###########################################################
# Installation                                            #
###########################################################

if(KNOODLE_BUILD_INSTALL)
  install(FILES
      "${CMAKE_CURRENT_BINARY_DIR}/source/knoodle_version.hpp"
      DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

  install(DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/config/"
    DESTINATION "config"
    FILES_MATCHING PATTERN "*.ini")

  set(CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
  )

  set(CPACK_PACKAGE_VENDOR "knoodle")
  set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
  set(CPACK_GENERATOR "Nuget,ZIP")
  set(CPACK_THREADS 0)

  include(CPack)
endif()
