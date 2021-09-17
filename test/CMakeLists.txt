cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(${ROOT_PROJECT_NAME}Tests LANGUAGES CXX)

# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage("gh:onqtam/doctest#2.4.5")
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.0")

if(TEST_INSTALLED_VERSION)
  find_package(${ROOT_PROJECT_NAME} REQUIRED)
else()
  CPMAddPackage(NAME ${ROOT_PROJECT_NAME} SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

include_directories(${CMAKE_BINARY_DIR}/_deps/include)
link_directories(${CMAKE_BINARY_DIR}/_deps/lib)

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${sources})
target_link_libraries(${PROJECT_NAME} doctest::doctest ${ROOT_PROJECT_NAME}::${ROOT_PROJECT_NAME} cli::cli dl)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wpedantic -Wextra)
  elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /W4 /WX)
    target_compile_definitions(${PROJECT_NAME}Tests PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add ${PROJECT_NAME}Tests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME projectNameTests COMMAND ${PROJECT_NAME}Tests)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME})

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(${PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(${PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
endif()
