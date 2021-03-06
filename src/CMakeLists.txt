# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${ROOT_PROJECT_NAME} INTERFACE)
add_library(${PROJECT_LIB_NAME} ${headers} ${sources})

set_target_properties(${PROJECT_LIB_NAME} PROPERTIES CXX_STANDARD 20)

# being a cross-platform target, we enforce standards conformance on MSVC
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(
      ${PROJECT_LIB_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->"
                                 -fno-strict-aliasing -DNDEBUG -Wall -O3 -Wno-variadic-macros
    )
  else()
    target_compile_options(
      ${PROJECT_LIB_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->"
    )
  endif()
  target_compile_definitions(${PROJECT_LIB_NAME} PUBLIC NDEBUG)
else()
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(
      ${PROJECT_LIB_NAME}
      PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->"
             -fno-strict-aliasing
             -ggdb
             -g3
             -DDEBUG
             -Wall
             -O0
             -Wno-variadic-macros
    )
  else()
    target_compile_options(
      ${PROJECT_LIB_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->"
    )
  endif()
  target_compile_definitions(${PROJECT_LIB_NAME} PUBLIC DEBUG)
endif()

# ------ Link dependencies --------

add_dependencies(${PROJECT_LIB_NAME} project_libuv project_aerospike)
# the dependencies need to be setup in the library setup so that the correct version'd headers files will be used (e.g. the library should use the compiled ssl headers not the system ssl headers)
if (ENABLE_STATIC_BUILD)
  foreach (dep IN LISTS ${PROJECT_NAME}_STATIC_DEPENDENCIES)
    add_dependencies(${PROJECT_LIB_NAME} ${dep})
  endforeach()
endif()

target_link_libraries(${PROJECT_LIB_NAME} PRIVATE ${PROJECT_LIBRARIES})

target_include_directories(
  ${PROJECT_LIB_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                             $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_LIB_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt ${libfmt_version}" "cli ${cli_version}"
)
