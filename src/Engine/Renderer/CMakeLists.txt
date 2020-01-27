cmake_minimum_required(VERSION 3.14)
project(Renderer CXX)
###################################################
add_library(OpenGlBackend INTERFACE)
###################################################

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

find_package(Stb)
if (NOT Stb_FOUND)
	install_package("stb" result)
	if (NOT result EQUAL 0)
		message(FATAL_ERROR "Could not install stb with error: ${result}")
	endif()
	find_package(Stb REQUIRED)
endif()

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  #Render folder
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseShader.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseTexture.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Camera.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/FrameBufferObject.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/FreeTypeFont.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/HdrTechnique.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/IndexBuffer.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Light.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Material.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/OpenglDebug.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Particles.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Pipeline.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Postprocessor.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/ReflectShader.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/GLRenderer.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseRenderer.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/ScreenShader.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shader.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/ShadowMapShader.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/ShadowMapTechnique.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/TechniqueManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Terrain.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Texture.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/TextureCube.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/VertexArrayObject.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/VertexBuffer.cpp
)

if (OPTION_GLAD_LOADER)
  target_sources(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/GladLoader.cpp
  )
endif()


target_link_libraries(${PROJECT_NAME} PUBLIC OpenGlBackend)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE Common)
target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
#target_include_directories(${OpenGlBackend} 
#	INTERFACE $<TARGET_PROPERTY:glad,INTERFACE_INCLUDE_DIRECTORIES>
#)

add_private_headers()
