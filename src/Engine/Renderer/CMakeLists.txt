cmake_minimum_required(VERSION 3.10)

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

#add_library(FxParser SHARED)

if (OPTION_DX_RENDERER)
	include("D3D/CMakeLists.txt")
endif()
if(OPTION_VK_RENDERER)
	include(${PROJECT_SOURCE_DIR}/Vulkan/CMakeLists.txt)
endif()

#add_subdirectory(Shaders)

start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	PROJECTS RendererDX RendererGL RendererVK
	SOURCE_GROUP "Source Files"
		"AuxRenderer.cpp"
		"AuxRendererNull.cpp"
		"BaseRenderer.cpp"
		"FreeTypeFont.cpp"
		"Shaders/FxParser.cpp"
 )

add_sources("NoUberFile"
	PROJECTS RendererDX
	SOURCE_GROUP "D3D"
		"D3D/Renderer.cpp"
		"D3D/BufferManager.cpp"
		"D3D/Shader.cpp"
		"D3D/AuxRenderer.h"
		"D3D/AuxRenderer.cpp"
)

add_sources("NoUberFile"
	PROJECTS RendererGL
	SOURCE_GROUP "OpenGL"
		"OpenGL/GladLoader.cpp"
		"OpenGL/GlewLoader.cpp"
		"OpenGL/OpenglDebug.cpp"
		"OpenGL/Renderer.cpp"
		"OpenGL/Renderer.hpp"
)
end_sources()

macro(add_common_libs)
	target_link_libraries(${THIS_PROJECT} PRIVATE Math)
	target_link_libraries(${THIS_PROJECT} PRIVATE freetype)
	target_link_libraries(${THIS_PROJECT} PRIVATE Utils)
	target_link_libraries(${THIS_PROJECT} PRIVATE FxParser)
	add_SDL(${THIS_PROJECT})
endmacro()

macro(add_common_includes)
	target_include_directories(${THIS_PROJECT} PRIVATE ${Stb_INCLUDE_DIR})
endmacro()

if(OPTION_DX_RENDERER)
	EngineModule(RendererDX FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER "Engine")
	add_common_libs()
	add_common_includes()

	target_link_libraries(${THIS_PROJECT} PRIVATE D3DBackend)
	target_include_directories(${THIS_PROJECT} PRIVATE ${D3D10_INCLUDE_DIRS})

	message(STATUS "renderer sources")
endif()

	#EngineModule(RendererGL FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER "Engine")
	#add_common_libs()
	#add_common_includes()
	#include(${CMAKE_CURRENT_SOURCE_DIR}/OpenGL/CMakeLists.txt)
	#target_link_libraries(${THIS_PROJECT} PUBLIC OpenGlBackend)

#[[
find_package(Vulkan)
if (Vulkan_FOUND)
	target_link_libraries(FxParser PUBLIC Vulkan::Vulkan)
	target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
endif()

source_group(
	TREE ${PROJECT_SOURCE_DIR}
	#PREFIX "Header Files"
	FILES
	${SOURCES}
)
]]
#add_private_headers()
