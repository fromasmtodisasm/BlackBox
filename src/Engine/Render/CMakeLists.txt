cmake_minimum_required(VERSION 3.15)
###################################################
add_library(OpenGlBackend INTERFACE)
###################################################

if (GLAD_LOADER)
  message(STATUS "Using Glad OpenGL loader")
  add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glad glad EXCLUDE_FROM_ALL)

  get_property(includes TARGET glad PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
  set_property(TARGET OpenGlBackend PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${includes})

  target_compile_definitions(OpenGlBackend INTERFACE -DGLAD_LOADER)
  target_compile_definitions(OpenGlBackend INTERFACE -DGLAD_GLAPI_EXPORT_BUILD)
elseif(GLEW_LOADER)                          
  find_package(GLEW REQUIRED)                
  target_compile_definitions(OpenGlBackend INTERFACE -DGLEW_LOADER)
endif()

#target_include_directories(${OpenGlBackend} 
#	INTERFACE $<TARGET_PROPERTY:glad,INTERFACE_INCLUDE_DIRECTORIES>
#)
