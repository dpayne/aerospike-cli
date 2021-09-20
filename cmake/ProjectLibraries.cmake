include(ExternalProject)

# PackageProject.cmake will be used to make our target installable
CPMAddPackage("gh:TheLartians/PackageProject.cmake@${package_project_version}")

CPMAddPackage(
  NAME fmt
  GIT_TAG ${libfmt_version}
  GITHUB_REPOSITORY fmtlib/fmt
  OPTIONS "FMT_INSTALL YES" # create an installable target
)

CPMAddPackage(
  NAME cli
  GIT_TAG ${cli_version}
  GITHUB_REPOSITORY daniele77/cli
)

ExternalProject_Add(
  project_libuv
  PREFIX ${EXTERNAL_PROJECTS_DIR}
  GIT_REPOSITORY "git@github.com:libuv/libuv.git"
  GIT_TAG ${libuv_version}
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX:PATH=${EXTERNAL_PROJECTS_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DBUILD_TESTING=OFF
    -DBUILD_SHARED_LIBS=OFF
    -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=${CMAKE_INTERPROCEDURAL_OPTIMIZATION}
    -DCMAKE_INSTALL_MESSAGE=LAZY
    -DCMAKE_LD_FLAGS=${CMAKE_LD_FLAGS}
    -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_INSTALL_LIBDIR=lib
    -DBUILD_SHARED_LIBS=OFF
)
add_library(uv STATIC IMPORTED)
set_property(
  TARGET uv PROPERTY IMPORTED_LOCATION
                     ${EXTERNAL_PROJECTS_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}uv_a.a
)

ExternalProject_Add(
  project_aerospike
  PREFIX ${EXTERNAL_PROJECTS_DIR}
  GIT_REPOSITORY "git@github.com:aerospike/aerospike-client-c.git"
  GIT_TAG ${aerospike_version}
  DEPENDS project_libuv
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ""
  BINARY_DIR ${EXTERNAL_PROJECTS_DIR}/src/project_aerospike
  # aerospike build breaks on parallel builds, so specify -j1 to disable parallel builds
  BUILD_COMMAND
    make -j1 EVENT_LIB=libuv
    EXT_CFLAGS=${CMAKE_C_FLAGS}\ -I${EXTERNAL_PROJECTS_DIR}/include\ -L${EXTERNAL_PROJECTS_DIR}/lib
  INSTALL_COMMAND
    cp ${EXTERNAL_PROJECTS_DIR}/src/project_aerospike/target/Linux-x86_64/lib/libaerospike.a
    ${EXTERNAL_PROJECTS_DIR}/lib/
  COMMAND
    cp -r
    ${EXTERNAL_PROJECTS_DIR}/src/project_aerospike/target/${CMAKE_SYSTEM_NAME}-x86_64/include/.
    ${EXTERNAL_PROJECTS_DIR}/include/
)
add_library(aerospike STATIC IMPORTED)
set_property(
  TARGET aerospike PROPERTY IMPORTED_LOCATION
                            ${EXTERNAL_PROJECTS_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}aerospike.a
)
