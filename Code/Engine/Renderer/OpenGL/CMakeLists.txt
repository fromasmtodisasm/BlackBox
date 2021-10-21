###################################################
add_library(OpenGlBackend INTERFACE)
###################################################

if (OPTION_GLAD_LOADER AND NOT Linux)
  message(STATUS "Using Glad OpenGL loader")
  add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glad glad EXCLUDE_FROM_ALL)
  target_link_libraries(OpenGlBackend INTERFACE glad)
###################################################
  get_property(includes TARGET glad PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
  set_property(TARGET OpenGlBackend PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${includes})
###################################################
  target_compile_definitions(OpenGlBackend INTERFACE -DGLAD_LOADER)
  target_compile_definitions(OpenGlBackend INTERFACE -DGLAD_GLAPI_EXPORT_BUILD)
elseif(OPTION_GLEW_LOADER)                          
  add_package(GLEW)                
  target_compile_definitions(OpenGlBackend INTERFACE -DGLEW_LOADER)
  target_link_libraries(OpenGlBackend INTERFACE GLEW::GLEW)
  if (Linux)
	  target_link_libraries(OpenGlBackend INTERFACE OpenGL)
  endif()
endif()
install_this(OpenGlBackend)
