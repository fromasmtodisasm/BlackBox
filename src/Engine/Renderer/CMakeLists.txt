cmake_minimum_required(VERSION 3.10)
project(Renderer CXX)

add_package(Stb)
find_package(freetype CONFIG REQUIRED)
# ===============> bison
find_package(BISON)
find_package(FLEX)
if((NOT BISON_FOUND) OR (NOT FLEX_FOUND))
    message(WARNING "
      No Bison and/or Flex available. FxParser will only use the already generated code.
      You can try to set BISON_EXECUTABLE & FLEX_EXECUTABLE"
    )
  set(BISON_EXECUTABLE "" CACHE PATHFILE "bison")
  set(FLEX_EXECUTABLE "" CACHE PATHFILE "flex")
  MARK_AS_ADVANCED(CLEAR BISON_EXECUTABLE)
  MARK_AS_ADVANCED(CLEAR FLEX_EXECUTABLE)
endif()

if(BISON_FOUND)
  BISON_TARGET(
	  ShaderParser Parser.yy ${PROJECT_SOURCE_DIR}/Parser.cpp
	  DEFINES_FILE ${PROJECT_SOURCE_DIR}/parser.hh
  )
endif()
if(FLEX_FOUND)
  FLEX_TARGET(ShaderScanner Scanner.yy ${PROJECT_SOURCE_DIR}/Scanner.cpp)
endif()
if(FLEX_FOUND AND BISON_FOUND)
  ADD_FLEX_BISON_DEPENDENCY(ShaderScanner ShaderParser)
endif()

add_library(${PROJECT_NAME})
add_library(FxParser SHARED)

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

  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/FxParser.cpp
)

target_sources(FxParser
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Parser.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Scanner.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Driver.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Parser.yy
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Scanner.ll
)

target_include_directories(FxParser PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(FxParser PRIVATE ${FLEX_INCLUDE_DIRS})
INSTALL_THIS(FxParser)


target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
#target_link_libraries(${PROJECT_NAME} PRIVATE FxParser)

add_SDL()

find_package(Vulkan REQUIRED)
target_link_libraries(FxParser PUBLIC Vulkan::Vulkan)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(FxParser PRIVATE ${CMAKE_SOURCE_DIR})


source_group(
	TREE ${PROJECT_SOURCE_DIR}
	#PREFIX "Header Files"
	FILES
	${SOURCES}
)
add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)

target_precompile_headers(FxParser
  PRIVATE 
  "pch.hpp"
)
