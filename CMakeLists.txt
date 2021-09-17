cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----

# Note: update this to your new project's name and version
project(
  ${ROOT_PROJECT_NAME}
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----

set(EXTERNAL_PROJECTS_DIR ${CMAKE_BINARY_DIR}/_deps)
include_directories(${EXTERNAL_PROJECTS_DIR}/include)
link_directories(${EXTERNAL_PROJECTS_DIR}/lib)
include(ProjectLibraries.cmake)

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${ROOT_PROJECT_NAME} INTERFACE)
add_library(${ROOT_PROJECT_NAME} ${headers} ${sources})

set_target_properties(${ROOT_PROJECT_NAME} PROPERTIES CXX_STANDARD 17)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(${ROOT_PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

add_dependencies(${ROOT_PROJECT_NAME} project_libuv project_aerospike)

target_link_directories(
    ${ROOT_PROJECT_NAME} PRIVATE ${EXTERNAL_PROJECTS_DIR}/lib
)

# Link dependencies
target_link_libraries(${ROOT_PROJECT_NAME} PRIVATE fmt::fmt cli::cli aerospike uv z ssl crypto)

target_include_directories(
    ${ROOT_PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths
string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
  DEPENDENCIES "fmt 7.1.3" "cli 2.0.0"
)
