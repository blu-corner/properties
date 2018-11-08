INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/sample.properties DESTINATION
        examples/properties)
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cpp/ DESTINATION
        examples/properties/cpp/)

if(JAVA)
    INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/java/ DESTINATION
            examples/properties/java/)
endif()

if(PYTHON)
    INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python/ DESTINATION
            examples/properties/python/)
endif()

if(CSHARP)
    INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/csharp/ DESTINATION
            examples/properties/csharp/)
endif()
