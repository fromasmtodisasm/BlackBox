cmake_minimum_required(VERSION 3.15)
project(Renderer CXX)
###################################################
add_library(OpenGlBackend INTERFACE)
###################################################

if (GLAD_LOADER)
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

add_library(Renderer)
target_sources(Renderer
  #Render folder
  PRIVATE ${SOURCE_DIR}/Engine/Render/BaseShader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/BaseTexture.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Camera.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/FrameBufferObject.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/FreeTypeFont.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/HdrTechnique.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/IndexBuffer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Light.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Material.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/OpenglDebug.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Particles.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Pipeline.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Postprocessor.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ReflectShader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/GLRenderer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/BaseRenderer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ScreenShader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Shader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ShadowMapShader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ShadowMapTechnique.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/TechniqueManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Terrain.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Texture.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/TextureCube.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/VertexArrayObject.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/VertexBuffer.cpp
)

if (GLAD_LOADER)
  target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCE_DIR}/Engine/Render/GladLoader.cpp
  )
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE OpenGlBackend)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE Common)
target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)

target_include_directories(${PROJECT_NAME} PRIVATE)
#target_include_directories(${OpenGlBackend} 
#	INTERFACE $<TARGET_PROPERTY:glad,INTERFACE_INCLUDE_DIRECTORIES>
#)

add_private_headers()
