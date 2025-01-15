cmake_minimum_required(VERSION 3.21)

###########################################################
# Global settings                                         #
###########################################################

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

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

###########################################################
# Dependencies                                            #
###########################################################

###########################################################
# Project Options                                         #
###########################################################

option(KNOODLE_BUILD_EXAMPLES "Build the examples" ON)

add_library(knoodle-options INTERFACE)

###########################################################
# Git Versioning                                          #
###########################################################

set(GIT_SHA "unknown" CACHE STRING "Git SHA of the current build")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

###########################################################
# Compiler settings                                       #
###########################################################

target_compile_features(knoodle-options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})

add_library(knoodle::knoodle-options ALIAS knoodle-options)

###########################################################
# Project versioning                                      #
###########################################################

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/include/knoodle/version.hpp.in"
  "${CMAKE_CURRENT_BINARY_DIR}/include/knoodle/version.hpp"
  ESCAPE_QUOTES)

###########################################################
# Add subdirectories                                      #
###########################################################

add_subdirectory(src)

include(CTest)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

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

include(CPack)
