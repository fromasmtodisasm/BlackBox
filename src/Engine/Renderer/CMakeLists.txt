cmake_minimum_required(VERSION 3.10)
if(GL_RENDERER)
	project(RendererGL CXX)
elseif(DX_RENDERER)
	project(RendererDX CXX)
endif()

add_package(Stb)
find_package(Freetype CONFIG REQUIRED)
# ===============> bison
find_package(BISON)
find_package(FLEX)
find_package(Cygwin)

macro(cygwin_bison INPUT OUTPUT)
	add_custom_command(
		OUTPUT ${OUTPUT} 
		COMMAND ${CYGWIN_INSTALL_PATH}/bin/bash --login -c "bison `cygpath -u ${INPUT}` -o `cygpath -u ${OUTPUT}`"
		DEPENDS ${INPUT}
	  ) 
endmacro()

macro(cygwin_flex INPUT OUTPUT)
	add_custom_command(
		OUTPUT ${OUTPUT} 
		COMMAND ${CYGWIN_INSTALL_PATH}/bin/bash --login -c "flex -o `cygpath -u ${OUTPUT}` `cygpath -u ${INPUT}`"
		DEPENDS ${INPUT}
	  ) 
endmacro()

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
	  ShaderParser ${PROJECT_SOURCE_DIR}/Shaders/Parser.yy ${PROJECT_SOURCE_DIR}/Shaders/Parser.cpp
	  #DEFINES_FILE ${PROJECT_SOURCE_DIR}/parser.hpp
  )
elseif (CYGWIN_INSTALL_PATH)
	cygwin_bison(${PROJECT_SOURCE_DIR}/Shaders/Parser.yy ${PROJECT_SOURCE_DIR}/Shaders/Parser.cpp)
endif()
if(FLEX_FOUND)
  FLEX_TARGET(ShaderScanner ${PROJECT_SOURCE_DIR}/Shaders/Scanner.ll ${PROJECT_SOURCE_DIR}/Shaders/Scanner.cpp)
elseif (CYGWIN_INSTALL_PATH)
	cygwin_flex(${PROJECT_SOURCE_DIR}/Shaders/Scanner.ll ${PROJECT_SOURCE_DIR}/Shaders/Scanner.cpp)
endif()
if(FLEX_FOUND AND BISON_FOUND)
  ADD_FLEX_BISON_DEPENDENCY(ShaderScanner ShaderParser)
endif()

add_library(${PROJECT_NAME})
add_library(FxParser SHARED)

if (DEFINED DX_RENDERER)
	include(${PROJECT_SOURCE_DIR}/D3D/CMakeLists.txt)
elseif (DEFINED GL_RENDERER)
	include(${PROJECT_SOURCE_DIR}/OpenGL/CMakeLists.txt)
elseif(VULKAN_RENDERER)
	include(${PROJECT_SOURCE_DIR}/Vulkan/CMakeLists.txt)
endif()

target_sources(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/AuxRenderer.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/AuxRendererNull.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseRenderer.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/FreeTypeFont.cpp
#[[
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseShader.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseShaderLoading.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BaseTexture.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/BufferManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Camera.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/FrameBufferObject.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Material.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Resources/Main.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Resources/ShaderManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Resources/TextureManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shader.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/TechniqueManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Texture.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/TextureCube.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/VertexBuffer.cpp

]]
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/FxParser.cpp
 )

target_sources(FxParser
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Parser.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Effect.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Scanner.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Driver.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Parser.yy
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/Scanner.ll
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/dummy.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Shaders/ScannerHelpers.cpp
 )

target_include_directories(FxParser PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(FxParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Shaders)
target_compile_definitions(FxParser PRIVATE "-DYY_NO_UNISTD_H")

#target_include_directories(FxParser PRIVATE ${FLEX_INCLUDE_DIRS})
INSTALL_THIS(FxParser)


target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE Utils)
target_link_libraries(${PROJECT_NAME} PRIVATE FxParser)

add_SDL()

find_package(Vulkan)
if (Vulkan_FOUND)
	target_link_libraries(FxParser PUBLIC Vulkan::Vulkan)
	target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
endif()
target_link_libraries(FxParser PUBLIC Math)
#target_link_libraries(FxParser PUBLIC OpenGlBackend)
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
