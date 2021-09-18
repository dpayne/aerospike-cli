# this script adds all subprojects to a single build to allow IDEs understand the full project
# structure.
set(ROOT_PROJECT_NAME "ascli")
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# options
option(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# ---- Add dependencies via CPM ----

include(cmake/tools.cmake)
include(cmake/ExternalDependencies.cmake)
include(cmake/CPM.cmake)
include(cmake/ProjectLibraries.cmake)

# needed to generate test target
enable_testing()

# My default build all
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src ${CMAKE_BINARY_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/main ${CMAKE_BINARY_DIR}/main)
# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test ${CMAKE_BINARY_DIR}/test)
# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/docs ${CMAKE_BINARY_DIR}/docs)
