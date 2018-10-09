#
# Copyright 2014-2018 Neueda Ltd.
#
include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_INSTALL_PREFIX}/include/utils
  ${CMAKE_INSTALL_PREFIX}/include/sbf
  )

link_directories(
    ${CMAKE_INSTALL_PREFIX}/lib
)

set(CONFIG_HEADERS
  properties.h
  )

install(FILES ${CONFIG_HEADERS}
  DESTINATION include/properties
  )

add_library(properties SHARED
  properties.cpp
  )

target_link_libraries(properties utils)
add_dependencies(properties UTILS_PROJECT)

install(TARGETS properties
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION lib
  )

install(
  EXPORT ${PROJECT_NAME}
  DESTINATION lib/cmake/${PROJECT_NAME} COMPONENT dev
  PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  )

set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(${PROJECT_NAME}_LIBRARIES properties)
configure_file(
  ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config-install.cmake
  @ONLY
  )
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config-install.cmake
  DESTINATION lib/cmake/${PROJECT_NAME} COMPONENT dev
  RENAME ${PROJECT_NAME}Config.cmake
  PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  )

add_subdirectory(bindings)
