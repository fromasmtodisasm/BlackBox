cmake_minimum_required(VERSION 3.10)
project(Renderer CXX)

add_package(Stb)
find_package(freetype CONFIG REQUIRED)
add_library(${PROJECT_NAME})

if (DEFINED GL_RENDERER)
	include(${PROJECT_SOURCE_DIR}/OpenGL/CMakeLists.txt)
elseif(VULKAN_RENDERER)
	include(${PROJECT_SOURCE_DIR}/Vulkan/CMakeLists.txt)
endif()

target_sources(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/AuxRenderer.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseRenderer.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseShader.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseShaderLoading.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseTexture.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BufferManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Camera.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/FrameBufferObject.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/FreeTypeFont.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/HdrTechnique.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/IndexBuffer.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Material.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Resources/Main.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Resources/ShaderManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Resources/TextureManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shader.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/TechniqueManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Terrain.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Texture.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/TextureCube.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/VertexBuffer.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)

add_SDL()

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)
