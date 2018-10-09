#
# Copyright 2014-2018 Neueda Ltd.
#
find_program(MCS mcs)
if(NOT MCS)
  message(FATAL_ERROR "Failed to find mono compiler")
endif()
message(STATUS "Found mono compiler: ${MCS}")

# set the flags for swig
set(CMAKE_SWIG_FLAGS -namespace Neueda.Config -dllimport PropertiesCsharp)
set_source_files_properties(../properties.csharp.i PROPERTIES CPLUSPLUS ON)

# Add swig module
add_compile_options(-fno-strict-aliasing)
swig_add_module(PropertiesCsharp csharp ../properties.csharp.i)
swig_link_libraries(PropertiesCsharp properties)

# Files to install with csharp
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Properties.dll
  COMMAND ${MCS} -target:library -out:${CMAKE_CURRENT_BINARY_DIR}/Properties.dll ${CMAKE_CURRENT_BINARY_DIR}/*.cs
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CSHARP_SRC_FILES}
  COMMENT "Generating Csharp DLL"
  VERBATIM
  )
add_custom_target(CsharpDLL ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Properties.dll)

if (TARGET _PropertiesCsharp)
  add_dependencies(CsharpDLL _PropertiesCsharp)
else()
  add_dependencies(CsharpDLL PropertiesCsharp)
endif()

if(APPLE)
  set(CSHARP_DYLIB_PREFIX "")
  set(CSHARP_DYLIB_SUFFIX ".so")
else()
  set(CSHARP_DYLIB_PREFIX "lib")
  set(CSHARP_DYLIB_SUFFIX ".so")
endif()

# cmake 2.8 swig has issues on figuring out naming libs
if(TARGET _PropertiesCsharp)
  set_target_properties(_PropertiesCsharp PROPERTIES OUTPUT_NAME "PropertiesCsharp")
  set_target_properties(_PropertiesCsharp PROPERTIES SUFFIX ${CSHARP_DYLIB_SUFFIX})
endif()

set(CSHARP_INSTALL_FILES
  # ${CMAKE_CURRENT_SOURCE_DIR}/run.sh
  # ${CMAKE_CURRENT_SOURCE_DIR}/build.sh
  # ${CMAKE_CURRENT_SOURCE_DIR}/example.cs
  ${CMAKE_CURRENT_BINARY_DIR}/Properties.dll)

# Declare install target for csharp
install(FILES ${CSHARP_INSTALL_FILES}
  DESTINATION lib/csharp)

if(TARGET _PropertiesCsharp)
  install(TARGETS _PropertiesCsharp
    DESTINATION lib/csharp)
else()
  install(TARGETS PropertiesCsharp
    DESTINATION lib/csharp)
endif()
