# --- Import dependencies ----

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 2.2.1
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_MAIN_NAME} ${sources})

set_target_properties(
  ${PROJECT_MAIN_NAME} PROPERTIES CXX_STANDARD 20 OUTPUT_NAME "${PROJECT_MAIN_NAME}"
)

target_link_libraries(${PROJECT_MAIN_NAME} ${PROJECT_NAME}::${PROJECT_LIB_NAME} cxxopts cli::cli dl)
