#
# Copyright 2014-2018 Neueda Ltd.
#

set(TEST_LIBRARIES
  properties
  gtest
  gmock
)

if(NOT WIN32)
	set(TEST_LIBRARIES ${TEST_LIBRARIES}
					   pthread)
endif()

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/test
  ${CMAKE_BINARY_DIR}/googletest/include
  ${CMAKE_INSTALL_PREFIX}/include/utils
  ${CMAKE_INSTALL_PREFIX}/include/sbf
  )

link_directories(
    ${CMAKE_INSTALL_PREFIX}/lib
    ${CMAKE_BINARY_DIR}/googletest/lib
)
  
add_executable(unittest
  unittest.cc
  TestProperties.cc
  )

target_link_libraries(unittest ${TEST_LIBRARIES})

add_test(NAME unittest
  COMMAND unittest --gtest_output=xml:../test.xml
)

install(TARGETS unittest
    RUNTIME DESTINATION bin)

if(PYTHON)
    install(FILES python/unittestpy
            DESTINATION bin
            PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ
            GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    add_test(NAME unittestpy
        COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH=${CMAKE_INSTALL_PREFIX}/lib/python
        ${PYTHON_EXECUTABLE} ${CMAKE_INSTALL_PREFIX}/bin/unittestpy
    )
endif(PYTHON)

if(JAVA)
    add_subdirectory(java)
endif(JAVA)
