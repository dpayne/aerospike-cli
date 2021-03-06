# --- Import dependencies ----

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION ${cxxopts_version}
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(${PROJECT_MAIN_NAME} ${sources})

set_target_properties(
  ${PROJECT_MAIN_NAME} PROPERTIES CXX_STANDARD 20 OUTPUT_NAME "${PROJECT_MAIN_NAME}"
)

add_dependencies(${PROJECT_MAIN_NAME} ${PROJECT_LIB_NAME})

target_link_libraries(${PROJECT_MAIN_NAME} ${PROJECT_NAME}::${PROJECT_LIB_NAME} cxxopts cli::cli dl)

add_custom_target(${PROJECT_NAME}_executable ALL
    COMMAND ln -sf ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Linking executable shortcut ${CMAKE_CURRENT_BINARY_DIR}"
        )
