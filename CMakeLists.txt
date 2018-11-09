#
# Copyright 2014-2018 Neueda Ltd.
#
cmake_minimum_required(VERSION 2.8.0)
project(properties CXX)

if(PYTHON_LIBRARY AND PYTHON_INCLUDE_DIR AND PYTHON_EXECUTABLE)
    set(SUBMODULE_FLAGS ${SUBMODULE_FLAGS} -DPYTHON_LIBRARY=${PYTHON_LIBRARY})
    set(SUBMODULE_FLAGS ${SUBMODULE_FLAGS} -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR})
    set(SUBMODULE_FLAGS ${SUBMODULE_FLAGS} -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE})
endif()

# declare submodule global
set(SUBMODULE_FLAGS)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
endif()

if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
  set(BUILD_SHARED_LIBS TRUE)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif()

if(APPLE)
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-undefined,dynamic_lookup")
  set(CMAKE_MACOSX_RPATH 1)
endif()

# options
option(DEBUG "Enable debug build" OFF)
option(JAVA "Enable Java bindings" OFF)
option(PYTHON "Enable Python bindings" OFF)
option(CSHARP "Enable C# bindings" OFF)
option(COVERAGE "Enable gcov coverage" OFF)
option(EXAMPLES "Enable examples" ON)

# set version info
set(CONFIG_MAJOR_VERSION 0)
set(CONFIG_MINOR_VERSION 2)
set(CONFIG_PATCH_VERSION 6)
set(CONFIG_VERSION ${CONFIG_MAJOR_VERSION}.${CONFIG_MINOR_VERSION}.${CONFIG_PATCH_VERSION})

if (UNIX)
  macro (add_library _name)
    _add_library(${ARGV})
    if (NOT APPLE AND TARGET ${_name})
      SET_TARGET_PROPERTIES(${_name}
        PROPERTIES
            VERSION ${CONFIG_VERSION}
            # SOVERSION 0.0.0
        )
    endif()
  endmacro()
endif (UNIX)

# compile options
add_compile_options(-Wall -c -g)
if (DEBUG)
  message(STATUS "properties DEBUG: ON")
  set(SUBMODULE_FLAGS ${SUBMODULE_FLAGS} -DDEBUG=on)
else ()
  message(STATUS "properties DEBUG: OFF")
  add_compile_options(-O3)
endif (DEBUG)
if (COVERAGE)
    set(COVERAGE_COMPILE_FLAGS "-g -O0 -coverage -fprofile-arcs -ftest-coverage")
    set(COVERAGE_LINK_FLAGS "-coverage -lgcov")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COVERAGE_COMPILE_FLAGS}" )
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${COVERAGE_LINK_FLAGS}" )
endif(COVERAGE)

# allow external projects
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# extra include and links
include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_INSTALL_PREFIX}/include/
  ${CMAKE_INSTALL_PREFIX}/include/sbf
  ${CMAKE_INSTALL_PREFIX}/include/utils
  ${CMAKE_INSTALL_PREFIX}/include/event2)
link_directories(${CMAKE_INSTALL_PREFIX}/lib)
#
# deps
if(NOT SUBMODULE_ROOT_DIR)
    set(SUBMODULE_ROOT_DIR ${PROJECT_SOURCE_DIR})
endif()
if(NOT SUBMODULE_PREFIX_DIR)
    set(SUBMODULE_PREFIX_DIR ${CMAKE_BINARY_DIR})
endif()

find_library(EVENT_LIB
  NAMES event
  HINTS "${CMAKE_INSTALL_PREFIX}/lib"
  )
find_library(EVENT_PTHREAD_LIB
  NAMES event_pthreads
  HINTS "${CMAKE_INSTALL_PREFIX}/lib"
  )
if (EVENT_LIB AND EVENT_PTHREAD_LIB)
  message(STATUS "event-found: " ${EVENT_LIB})
  message(STATUS "event-pthread-found: " ${EVENT_PTHREAD_LIB})
else()
  set(LIBEVENT_CMAKE ${SUBMODULE_ROOT_DIR}/ext/libevent CACHE PATH "Location of libevent-cmake" FORCE)
  message(STATUS "libevent-location: " ${LIBEVENT_CMAKE})
  ExternalProject_Add(
    libevent
    PREFIX ${SUBMODULE_PREFIX_DIR}
    SOURCE_DIR ${LIBEVENT_CMAKE}
    CMAKE_ARGS -DEVENT__DISABLE_TESTS=ON -DEVENT__DISABLE_REGRESS=ON -DEVENT__DISABLE_OPENSSL=ON -DEVENT__DISABLE_SAMPLES=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    )
endif()

# sbf
set(SBF_CMAKE ${SUBMODULE_ROOT_DIR}/ext/sbf CACHE PATH "Location of sbf-cmake" FORCE)
message(STATUS "sbf-location: " ${SBF_CMAKE})
ExternalProject_Add(
    SBF_PROJECT
    PREFIX ${SUBMODULE_PREFIX_DIR}
    SOURCE_DIR ${SBF_CMAKE}
    CMAKE_ARGS -DSUBMODULE_PREFIX_DIR=${SUBMODULE_PREFIX_DIR} -DSUBMODULE_ROOT_DIR=${SUBMODULE_ROOT_DIR} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} ${SUBMODULE_FLAGS}
    )
ExternalProject_Get_Property(SBF_PROJECT binary_dir)
set(SBF_PROJECT_DIR ${binary_dir}/lib/cmake/sbf CACHE PATH "location of sbf cmake files" FORCE)
if (NOT EVENT_LIB)
    add_dependencies(SBF_PROJECT libevent)
endif()
set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS} "${binary_dir};sbf;ALL;/")

# utils
set(UTILS_CMAKE ${SUBMODULE_ROOT_DIR}/ext/utils CACHE PATH "Location of utils-cmake" FORCE)
message(STATUS "utils-location: " ${UTILS_CMAKE})
ExternalProject_Add(
    UTILS_PROJECT
    PREFIX ${SUBMODULE_PREFIX_DIR}
    SOURCE_DIR ${UTILS_CMAKE}
    CMAKE_ARGS -DSUBMODULE_PREFIX_DIR=${SUBMODULE_PREFIX_DIR} -DSUBMODULE_ROOT_DIR=${SUBMODULE_ROOT_DIR} -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} ${SUBMODULE_FLAGS}
    )
ExternalProject_Get_Property(UTILS_PROJECT binary_dir)
add_dependencies(UTILS_PROJECT SBF_PROJECT)
set(UTILS_PROJECT_DIR ${binary_dir}/lib/cmake/utils CACHE PATH "location of utils cmake files" FORCE)
set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS} "${binary_dir};utils;ALL;/")

# add sources
add_subdirectory(src)

# unit-tests
option(TESTS "Enable unit-tests" OFF)
if(TESTS)
  if(WIN32)
    ExternalProject_Add(
      googletest
      SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/googletest
      CMAKE_ARGS -Dgtest_force_shared_crt=ON -DCMAKE_CXX_FLAGS="/w" -DBUILD_GMOCK=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/googletest -DCMAKE_INSTALL_LIBDIR=${CMAKE_BINARY_DIR}/googletest/lib
      )
  else()
    ExternalProject_Add(
      googletest
      SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/googletest
      CMAKE_ARGS -DBUILD_GMOCK=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/googletest -DCMAKE_INSTALL_LIBDIR=${CMAKE_BINARY_DIR}/googletest/lib
      )
  endif()
  
  include(CTest)
  enable_testing()
  
  # add test dir
  add_subdirectory(test)
endif()

# add examples
if(EXAMPLES)
    add_subdirectory(examples)
endif()

set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS} "${CMAKE_BINARY_DIR};properties;ALL;/")
set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_VERSION_MAJOR "${CONFIG_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR "${CONFIG_MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH "${CONFIG_PATCH_VERSION}")
set(CPACK_PACKAGE_NAME "properties")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
if(DEBUG)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_dbg-${CPACK_PACKAGE_VERSION}")
else()
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
endif()

INCLUDE(CPack)
