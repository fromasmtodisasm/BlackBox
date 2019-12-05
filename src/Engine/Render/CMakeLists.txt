cmake_minimum_required(VERSION 3.15)
project(OpenGlBackend CXX)
###################################################
add_library(${PROJECT_NAME} INTERFACE IMPORTED)
###################################################

if (GLAD_LOADER)
  add_subdirectory(${CMAKE_SOURCE_DIR}/lib lib EXCLUDE_FROM_ALL)
  target_compile_definitions(${PROJECT_NAME} INTERFACE -DGLAD_LOADER)
  target_compile_definitions(${PROJECT_NAME} INTERFACE -DGLAD_GLAPI_EXPORT_BUILD)
elseif(GLEW_LOADER)                          
  find_package(GLEW REQUIRED)                
  target_compile_definitions(${PROJECT_NAME} INTERFACE -DGLEW_LOADER)
endif()

target_include_directories(${PROJECT_NAME} 
	INTERFACE $<TARGET_PROPERTY:glad,INTERFACE_INCLUDE_DIRECTORIES>
)
