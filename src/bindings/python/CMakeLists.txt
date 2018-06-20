#
# Copyright 2014-2018 Neueda Ltd.
#
# Include python
find_package(PythonInterp REQUIRED)
set(Python_ADDITIONAL_VERSIONS 2.7)
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

# set the flags for swig
set(CMAKE_SWIG_FLAGS -module properties)
set_source_files_properties(../properties.py.i PROPERTIES CPLUSPLUS ON)

# Add swig module
swig_add_module(Conf python ../properties.py.i)
swig_link_libraries(Conf properties ${PYTHON_LIBRARIES})
set_target_properties(_Conf PROPERTIES OUTPUT_NAME "_properties")

# Files to install with Python
set(PYTHON_INSTALL_FILES
  # ${CMAKE_CURRENT_SOURCE_DIR}/example.py
  ${CMAKE_CURRENT_BINARY_DIR}/properties.py
  ${CMAKE_CURRENT_BINARY_DIR}/_properties.so)

install(FILES ${PYTHON_INSTALL_FILES}
        DESTINATION lib/python)

set(SWIG_PY_RUNTIME ${CMAKE_INSTALL_PREFIX}/include/swigPyRuntime.h)
if(NOT EXISTS ${SWIG_PY_RUNTIME})
    install(CODE "execute_process(COMMAND swig -c++ -python -external-runtime ${SWIG_PY_RUNTIME})")
endif(NOT EXISTS ${SWIG_PY_RUNTIME})
