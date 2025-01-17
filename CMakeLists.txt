cmake_minimum_required(VERSION 3.28)

###########################################################
# Global settings                                         #
###########################################################

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

###########################################################
# Project settings                                        #
###########################################################

project(
  "knoodle"
  VERSION 0.0.1
  DESCRIPTION "A tool for creating procedural textures using a node-based graph editor."
  HOMEPAGE_URL "knoodlegraph.org"
  LANGUAGES CXX C)

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

###########################################################
# Project Options                                         #
###########################################################

option(KNOODLE_BUILD_EXAMPLES "Build the examples" ON)
option(CODE_COVERAGE "Enable coverage reporting" OFF)

###########################################################
# Git Versioning                                          #
###########################################################

set(GIT_SHA "unknown" CACHE STRING "Git SHA of the current build")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

###########################################################
# Project versioning                                      #
###########################################################

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/version.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/version.hpp"
  ESCAPE_QUOTES)

###########################################################
# Add intern subdirectories                               #
###########################################################

add_subdirectory(src)

###########################################################
# Installation                                            #
###########################################################

if(CMAKE_SKIP_INSTALL_RULES)
  return()
endif()

include(GNUInstallDirs)

set(CPACK_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

set(CPACK_PACKAGE_VENDOR "knoodle")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_GENERATOR "Nuget,ZIP")
set(CPACK_THREADS 0)

include(CPack)
