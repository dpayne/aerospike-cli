# ---- Options ----

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# ---- Dependencies ----

CPMAddPackage("gh:onqtam/doctest#${doctest_version}")
CPMAddPackage("gh:TheLartians/Format.cmake@${cmake_format_version}")

# ---- Create binary ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_TEST_NAME} ${sources})
target_link_libraries(
  ${PROJECT_TEST_NAME} doctest::doctest ${PROJECT_NAME}::${PROJECT_LIB_NAME} cli::cli dl
)
set_target_properties(${PROJECT_TEST_NAME} PROPERTIES CXX_STANDARD 20)

# enable compiler warnings
if(NOT TEST_INSTALLED_VERSION)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_TEST_NAME} PUBLIC -Wall -Wextra)
  elseif(MSVC)
    target_compile_options(${PROJECT_TEST_NAME} PUBLIC /W4 /WX)
    target_compile_definitions(${PROJECT_TEST_NAME}Tests PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif()
endif()

# ---- Add ${PROJECT_TEST_NAME}Tests ----

enable_testing()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME projectNameTests COMMAND
# ${PROJECT_TEST_NAME})

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_TEST_NAME})

# ---- code coverage ----

if(ENABLE_TEST_COVERAGE)
  target_compile_options(${PROJECT_TEST_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options(${PROJECT_TEST_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
endif()
