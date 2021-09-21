set(EXTERNAL_PROJECTS_DIR ${CMAKE_BINARY_DIR}/_deps)
set(EXTERNAL_PROJECTS_INCLUDE_DIR  ${EXTERNAL_PROJECTS_DIR}/include)
set(EXTERNAL_PROJECTS_LIB_DIR ${EXTERNAL_PROJECTS_DIR}/lib)

if(APPLE)
    set(OPENSSL_ROOT_DIR /usr/local/opt/openssl@1.1/)
    set(OPENSSL_INCLUDE_DIR /usr/local/opt/openssl@1.1/include)
    list(APPEND OPENSSL_LIBRARIES ssl crypto)
else()
    set(OPENSSL_ROOT_DIR /usr/lib/)
    find_package(OpenSSL)
endif()

# Release builds will build openssl from source
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(OPENSSL_ROOT_DIR ${EXTERNAL_PROJECTS_DIR})
    set(OPENSSL_INCLUDE_DIR ${EXTERNAL_PROJECTS_INCLUDE_DIR})
    set(OPENSSL_LIBRARIES crypto ssl)
endif()

include_directories(${EXTERNAL_PROJECTS_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
link_directories(${EXTERNAL_PROJECTS_LIB_DIR} ${OPENSSL_ROOT_DIR}/lib)
