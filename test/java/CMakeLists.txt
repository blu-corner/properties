set(TARGET ${CMAKE_BINARY_DIR}/test/java/target/unittestj-1.0.0-jar-with-dependencies.jar)

add_custom_command(OUTPUT ${TARGET}
    COMMAND mvn -DskipTests package
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS PropertiesJNI ${CMAKE_CURRENT_SOURCE_DIR}/src/main/java/com/neueda/properties/test/)

add_custom_target(unittestj ALL
    DEPENDS ${TARGET})

add_dependencies(unittestj PropertiesJNI)

install(FILES ${TARGET}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
        RENAME unittest.jar)

add_test(NAME unittest.jar
    COMMAND java -cp
    ${CMAKE_INSTALL_PREFIX}/bin/unittest.jar:${CMAKE_INSTALL_PREFIX}/lib/java/PropertiesJNI.jar
    com.neueda.properties.test.Main
    )
