# this script adds all subprojects to a single build to allow IDEs understand the full project
# structure.
set(ROOT_PROJECT_NAME "ascli")
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

# options
option(ENABLE_TEST "Enable unit tests" ON)
option(ENABLE_DOCS "Enable documentation generation" ON)
option(ENABLE_MAIN "Enable main executable" ON)
option(ENABLE_STATIC_BUILD "Enable static build" OFF)
option(ENABLE_LINTER "Enable linter" OFF)
option(USE_CCACHE_BY_DEFAULT "Enable ccache" ON)

# external dependencies
set(EXTERNAL_PROJECTS_DIR ${CMAKE_BINARY_DIR}/_deps)
set(EXTERNAL_PROJECTS_INCLUDE_DIR  ${EXTERNAL_PROJECTS_DIR}/include)
set(EXTERNAL_PROJECTS_LIB_DIR ${EXTERNAL_PROJECTS_DIR}/lib)

# always export compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# by default enable clang tidy
if(ENABLE_LINTER)
  set(USE_STATIC_ANALYZER clang-tidy)
endif()

# enable ccache by default if available
if(USE_CCACHE_BY_DEFAULT)
  find_program(CCACHE_FOUND ccache)
  if(CCACHE_FOUND)
    set(USE_CCACHE ON CACHE BOOL "" FORCE)
  endif(CCACHE_FOUND)
endif()

# ---- Project ----

# Note: update this to your new project's name and version
project(
  ${ROOT_PROJECT_NAME}
  VERSION 1.0
  LANGUAGES CXX
)
set(PROJECT_MAIN_NAME ${ROOT_PROJECT_NAME})
set(PROJECT_LIB_NAME ${ROOT_PROJECT_NAME}Lib)
set(PROJECT_TEST_NAME ${ROOT_PROJECT_NAME}Test)
set(PROJECT_DOCS_NAME ${ROOT_PROJECT_NAME}Docs)

# ---- General Compile Time Flags ----
include(versions.cmake)
include(cmake/OpenSSL.cmake)
include(cmake/BuildFlags.cmake)

# ---- Add dependencies via CPM ----

include(ExternalProject)
include(cmake/tools.cmake)
include(cmake/CPM.cmake)

if(ENABLE_STATIC_BUILD)
  message("Building static executable")
  include(cmake/StaticBuild.cmake)
else()
  message("Building dynamic executable")
endif()

include(cmake/ProjectLibraries.cmake)

# My default build all
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src ${CMAKE_BINARY_DIR}/src)
if(${ENABLE_MAIN})
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/main ${CMAKE_BINARY_DIR}/main)
endif()

if(${ENABLE_TEST})
  # needed to generate test target
  enable_testing()
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test ${CMAKE_BINARY_DIR}/test)
endif()

if(${ENABLE_DOCS})
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/docs ${CMAKE_BINARY_DIR}/docs)
endif()
