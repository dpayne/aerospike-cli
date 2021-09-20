if(NOT DEFINED BUILD_FLAGS OR BUILD_FLAGS STREQUAL "")
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BUILD_FLAGS
        "-msse -msse2 -msse3 -mssse3 -mno-sse4a -maes -mpopcnt -mavx -mavx2 -msse4.2 -msse4.1 -mlzcnt -O2"
    )
  else()
    set(BUILD_FLAGS
        "-msse -msse2 -msse3 -mssse3 -mno-sse4a -maes -mpopcnt -mavx -mavx2 -msse4.2 -msse4.1 -mlzcnt -ggdb -g3"
    )
  endif()
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${BUILD_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BUILD_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${BUILD_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${BUILD_FLAGS}")

set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} ${BUILD_FLAGS}")
set(CMAKE_LD_FLAGS_DEBUG "${CMAKE_LD_FLAGS_DEBUG} ${BUILD_FLAGS}")
set(CMAKE_LD_FLAGS_RELEASE "${CMAKE_LD_FLAGS_RELEASE} ${BUILD_FLAGS}")

if(ENABLE_STATIC_BUILD)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  set(BUILD_SHARED_LIBS OFF)
  set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()
