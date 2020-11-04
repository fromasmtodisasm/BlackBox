###################################################
add_library(OpenGlBackend INTERFACE)
###################################################

set(GL_DIR ${PROJECT_SOURCE_DIR}/OpenGL)

if (OPTION_GLAD_LOADER)
  message(STATUS "Using Glad OpenGL loader")
  add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glad glad EXCLUDE_FROM_ALL)
  target_link_libraries(OpenGlBackend INTERFACE glad)
###################################################
  get_property(includes TARGET glad PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
  set_property(TARGET OpenGlBackend PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${includes})
###################################################
  target_compile_definitions(OpenGlBackend INTERFACE -DGLAD_LOADER)
  target_compile_definitions(OpenGlBackend INTERFACE -DGLAD_GLAPI_EXPORT_BUILD)
elseif(GLEW_LOADER)                          
  find_package(GLEW REQUIRED)                
  target_compile_definitions(OpenGlBackend INTERFACE -DGLEW_LOADER)
endif()
install_this(OpenGlBackend)

target_sources(${PROJECT_NAME}
  PRIVATE ${GL_DIR}/GLRenderer.cpp
  PRIVATE ${GL_DIR}/OpenglDebug.cpp
)

if (OPTION_GLAD_LOADER)
  target_sources(${PROJECT_NAME}
    PRIVATE ${GL_DIR}/GladLoader.cpp
  )
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGlBackend)
