#
# Copyright 2014-2018 Neueda Ltd.
#
cmake_minimum_required(VERSION 2.8.0)
project(properties CXX)

# declare submodule global
set(SUBMODULE_FLAGS)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
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
option(EXAMPLES "Enable examples" OFF)

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
add_compile_options(-Wall)
if (DEBUG)
  message(STATUS "properties DEBUG: ON")
  add_compile_options(-g3)
  set(SUBMODULE_FLAGS ${SUBMODULE_FLAGS} -DDEBUG=on)
else ()
  message(STATUS "properties DEBUG: OFF")
  add_compile_options(-g -O2)
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

# utils
set(utils_DIR ${CMAKE_INSTALL_PREFIX}/lib/cmake/utils)
find_package(utils QUIET)
if(NOT ${utils_FOUND})
  set(UTILS_CMAKE ${PROJECT_SOURCE_DIR}/ext/utils CACHE PATH "Location of utils-cmake" FORCE)
  message(STATUS "utils-location: " ${UTILS_CMAKE})
  ExternalProject_Add(
    UTILS_PROJECT
    SOURCE_DIR ${UTILS_CMAKE}
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} ${SUBMODULE_FLAGS}
    )
  ExternalProject_Get_Property(UTILS_PROJECT binary_dir)
  set(UTILS_PROJECT_DIR ${binary_dir}/lib/cmake/utils CACHE PATH "location of utils cmake files" FORCE)
endif()

# add sources
add_subdirectory(src)

# unit-tests
option(TESTS "Enable unit-tests" OFF)
if(TESTS)
  ExternalProject_Add(
    googletest
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/ext/googletest
    CMAKE_ARGS -DBUILD_GMOCK=ON -DBUILD_GTEST=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/googletest -DCMAKE_INSTALL_LIBDIR=${CMAKE_BINARY_DIR}/googletest/lib
    )
  
  include(CTest)
  enable_testing()
  
  # add test dir
  add_subdirectory(test)
endif()

INCLUDE(CPack)
SET(CPACK_GENERATOR "TGZ")

# add examples
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/examples/sample.properties DESTINATION
        ${CMAKE_INSTALL_PREFIX}/examples/)
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/cpp/ DESTINATION
        ${CMAKE_INSTALL_PREFIX}/examples/cpp/)

if(JAVA)
    INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/java/ DESTINATION
            ${CMAKE_INSTALL_PREFIX}/examples/java/)
endif()

if(PYTHON)
    INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/python/ DESTINATION
            ${CMAKE_INSTALL_PREFIX}/examples/python/)
endif()
