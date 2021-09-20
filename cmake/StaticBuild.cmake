# Openssl is built from source on release builds so we need to set the dependencies on openssl for projects that use it.
set(OPENSSL_FROM_SOURCE_PROJECT project_openssl)

ExternalProject_Add(project_lua
  EXCLUDE_FROM_ALL TRUE
  PREFIX ${EXTERNAL_PROJECTS_DIR}
  URL "https://www.lua.org/ftp/lua-${lua_version}.tar.gz"
  URL_HASH MD5=${lua_md5}
  BINARY_DIR ${EXTERNAL_PROJECTS_DIR}/src/project_lua
  CONFIGURE_COMMAND ""
  BUILD_COMMAND $(MAKE) linux
  INSTALL_COMMAND cp ${EXTERNAL_PROJECTS_DIR}/src/project_lua/src/liblua.a ${EXTERNAL_PROJECTS_LIB_DIR}/liblua.a
)

ExternalProject_Add(project_libz
  EXCLUDE_FROM_ALL TRUE
  PREFIX ${EXTERNAL_PROJECTS_DIR}
  GIT_REPOSITORY "git@github.com:madler/zlib.git"
  GIT_TAG ${libz_version}
  BINARY_DIR ${EXTERNAL_PROJECTS_DIR}/src/project_libz
  CONFIGURE_COMMAND prefix=${EXTERNAL_PROJECTS_DIR} ./configure --static
  BUILD_COMMAND $(MAKE)
  INSTALL_COMMAND $(MAKE) install
)

ExternalProject_Add(project_openssl
  EXCLUDE_FROM_ALL TRUE
  PREFIX ${EXTERNAL_PROJECTS_DIR}
  DEPENDS project_libz
  GIT_REPOSITORY "git@github.com:openssl/openssl.git"
  GIT_TAG ${openssl_version}
  BINARY_DIR ${EXTERNAL_PROJECTS_DIR}/src/project_openssl
  CONFIGURE_COMMAND ./config --prefix=${EXTERNAL_PROJECTS_DIR} --openssldir=${EXTERNAL_PROJECTS_DIR} --libdir=lib no-shared enable-zlib threads enable-md2 enable-rc5 enable-tls enable-tls1_3 enable-tls1_2 enable-tls1_1 "-Wa,--noexecstack -I${EXTERNAL_PROJECTS_INCLUDE_DIR} ${CMAKE_C_FLAGS} ${CMAKE_LD_FLAGS}"
  BUILD_COMMAND $(MAKE) build_libs
  # note: skipping doc install
  INSTALL_COMMAND $(MAKE) install_sw
)

ExternalProject_Add(project_jemalloc
  EXCLUDE_FROM_ALL TRUE
  PREFIX ${EXTERNAL_PROJECTS_DIR}
  GIT_REPOSITORY "git@github.com:jemalloc/jemalloc.git"
  GIT_TAG ${jemalloc_version}
  BINARY_DIR ${EXTERNAL_PROJECTS_DIR}/src/project_jemalloc
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ./autogen.sh --prefix=${EXTERNAL_PROJECTS_DIR} --disable-libdl --with-jemalloc-prefix=je_
  BUILD_COMMAND $(MAKE)
  # jemalloc install breaks on install_doc so skip the doc install
  INSTALL_COMMAND $(MAKE) install_bin install_include install_lib
)

add_custom_target(setup_glib_build_folder)
# glibc requires a separate build folder
add_custom_command(
  TARGET setup_glib_build_folder
  COMMAND mkdir -p ${EXTERNAL_PROJECTS_DIR}/src/project_glibc/build
)
ExternalProject_Add(project_glibc
  EXCLUDE_FROM_ALL TRUE
  PREFIX ${EXTERNAL_PROJECTS_DIR}
  DEPENDS setup_glib_build_folder
  GIT_REPOSITORY "git://sourceware.org/git/glibc.git"
  GIT_TAG ${glibc_version}
  BINARY_DIR ${EXTERNAL_PROJECTS_DIR}/src/project_glibc/build
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND mkdir -p ${EXTERNAL_PROJECTS_DIR}/src/project_glibc/build
  COMMAND ${EXTERNAL_PROJECTS_DIR}/src/project_glibc/configure --prefix=${EXTERNAL_PROJECTS_DIR} --enable-stack-protector=strong --enable-static-nss --with-headers="${EXTERNAL_PROJECTS_DIR}/src/project_glibc/include" --enable-add-ons --enable-bind-now --enable-cet --enable-lock-elision --enable-kernel=4.4 --enable-multi-arch --enable-stackguard-randomization --enable-static-pie --enable-systemtap --disable-profile --disable-werror
  BUILD_COMMAND make
  INSTALL_COMMAND make install
)

add_library(jemalloc STATIC IMPORTED)
add_library(lua STATIC IMPORTED)
add_library(z STATIC IMPORTED)
add_library(ssl STATIC IMPORTED)
add_library(crypto STATIC IMPORTED)

set_property(TARGET jemalloc PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}jemalloc.a)
set_property(TARGET lua PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}lua.a)
set_property(TARGET z PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}z.a)
set_property(TARGET ssl PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}ssl.a)
set_property(TARGET crypto PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}crypto.a)


# ---------------------- glib libs ----------------------
add_library(rt_nonshared STATIC IMPORTED)
add_library(c_nonshared STATIC IMPORTED)
add_library(crypt_nonshared STATIC IMPORTED)
add_library(resolv_nonshared STATIC IMPORTED)
add_library(m_nonshared STATIC IMPORTED)
add_library(mvec_nonshared STATIC IMPORTED)
add_library(dl_nonshared STATIC IMPORTED)

# Note we purposely create two libraries for pthreads. This is used later to fix a circular link dependency
add_library(pthread_nonshared STATIC IMPORTED)
add_library(pthread STATIC IMPORTED)

set_property(TARGET rt_nonshared PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}rt.a)
set_property(TARGET c_nonshared PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}c_nonshared.a)
set_property(TARGET crypt_nonshared PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}crypt.a)
set_property(TARGET resolv_nonshared PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}resolv.a)
set_property(TARGET m_nonshared PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}m.a)
set_property(TARGET mvec_nonshared PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}mvec.a)
set_property(TARGET dl_nonshared PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}dl.a)
set_property(TARGET pthread_nonshared PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}pthread.a)
set_property(TARGET pthread PROPERTY IMPORTED_LOCATION ${EXTERNAL_PROJECTS_LIB_DIR}/${CMAKE_FIND_LIBRARY_PREFIXES}pthread.a)

# ---------------------- list dependencies ----------------------
list(APPEND ${PROJECT_NAME}_STATIC_DEPENDENCIES project_lua project_libz project_openssl project_jemalloc project_glibc)
