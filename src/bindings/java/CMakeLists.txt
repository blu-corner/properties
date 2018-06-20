#
# Copyright 2014-2018 Neueda Ltd.
#
# Include Java
find_package(Java REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

include_directories(${JNI_INCLUDE_DIRS})

# set the flags for swig
set(CMAKE_SWIG_FLAGS -package com.neueda.properties)
set_source_files_properties(../properties.java.i PROPERTIES CPLUSPLUS ON)

# Add swig module
add_compile_options(-fno-strict-aliasing)
swig_add_module(Properties java ../properties.java.i)
swig_link_libraries(Properties properties ${JNI_LIBRARIES})

set(CMAKE_JAR_CLASSES_PREFIX com/neueda/properties)
set(JNI_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/propertiesBinding.java
    ${CMAKE_CURRENT_BINARY_DIR}/propertiesBindingJNI.java
    ${CMAKE_CURRENT_BINARY_DIR}/StringVector.java
    ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_std__string.java
    ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_bool.java
    ${CMAKE_CURRENT_BINARY_DIR}/SWIGTYPE_p_void.java
    ${CMAKE_CURRENT_BINARY_DIR}/Properties.java
    )

add_jar(PropertiesJNI
  ${JNI_SOURCES}
  )

if(TARGET _Properties)
  add_dependencies(PropertiesJNI _Properties)
else()
  add_dependencies(PropertiesJNI Properties)
endif()

# Files to install with Java
if(APPLE)
  set(JAVA_DYLIB_SUFFIX ".jnilib")
else()
  set(JAVA_DYLIB_SUFFIX ".so")
endif()

# cmake 2.8 swig has issues on figuring out naming libs
if(TARGET _properties)
  set_target_properties(_Properties PROPERTIES OUTPUT_NAME "Properties")
  set_target_properties(_Properties PROPERTIES SUFFIX ${JAVA_DYLIB_SUFFIX})
endif()

set(JAVA_INSTALL_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/PropertiesJNI.jar)

# Declare install target for java
install(FILES ${JAVA_INSTALL_FILES}
  DESTINATION lib/java)

if(TARGET _Properties)
  install(TARGETS _properties
    DESTINATION lib/java)
else()
  install(TARGETS Properties
    DESTINATION lib/java)
endif()
