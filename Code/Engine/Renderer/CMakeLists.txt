cmake_minimum_required(VERSION 3.10)

add_package(Stb)
add_package(freetype)
add_package(freeimage)
# ===============> bison
find_package(BISON)
find_package(FLEX)
find_package(Cygwin)

macro(cygwin_bison INPUT OUTPUT)
	add_custom_command(
		OUTPUT ${OUTPUT}
		COMMAND ${CYGWIN_INSTALL_PATH}/bin/bash --login -c "bison `cygpath -u ${INPUT}` -o `cygpath -u ${OUTPUT}`"
		DEPENDS ${INPUT})
endmacro()

macro(cygwin_flex INPUT OUTPUT)
	add_custom_command(
		OUTPUT ${OUTPUT}
		COMMAND ${CYGWIN_INSTALL_PATH}/bin/bash --login -c "flex `cygpath -u ${INPUT}` -o `cygpath -u ${OUTPUT}`"
		DEPENDS ${INPUT})
endmacro()

if((NOT BISON_FOUND) OR (NOT FLEX_FOUND))
	message(
		WARNING
			"
			No Bison and/or Flex available. FxParser will only use the already generated code.
			You can try to set BISON_EXECUTABLE & FLEX_EXECUTABLE")
	set(BISON_EXECUTABLE
			""
			CACHE PATHFILE "bison")
	set(FLEX_EXECUTABLE
			""
			CACHE PATHFILE "flex")
	mark_as_advanced(CLEAR BISON_EXECUTABLE)
	mark_as_advanced(CLEAR FLEX_EXECUTABLE)
endif()



# add_library(FxParser SHARED)

add_subdirectory(Shaders)

start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	RendererDX
	RendererGL
	RendererVK
	SOURCE_GROUP
	"Source Files"
	"AuxRenderer.cpp"
	"AuxRendererNull.cpp"
	"BaseRenderer.cpp"
	"FreeTypeFont.cpp"
	"Shaders/FxParser.cpp")

add_sources(
	"NoUberFile"
	PROJECTS
	RendererDX
	SOURCE_GROUP
	"D3D"
	"D3D/Renderer.cpp"
	"D3D/BufferManager.cpp"
	"D3D/Shader.cpp"
	"D3D/DDSTextureLoader.h"
	"D3D/DDSTextureLoader.cpp"
	)

add_sources(
	"NoUberFile"
	PROJECTS
	RendererGL
	SOURCE_GROUP
	"OpenGL"
	"OpenGL/GladLoader.cpp"
	"OpenGL/GlewLoader.cpp"
	"OpenGL/OpenglDebug.cpp"
	"OpenGL/Renderer.cpp"
	"OpenGL/Renderer.hpp")
add_sources(
	"NoUberFile"
	PROJECTS
	RendererVK
	SOURCE_GROUP
	"Vulkan"
	"Vulkan/Renderer.cpp"
	"Vulkan/Shader.cpp"
	"Vulkan/Renderer.h")
sources_platform(WINDOWS)
add_sources("NoUberFile" PROJECTS RendererVK SOURCE_GROUP
						"Vulkan\\\\Resource Files" "Vulkan/RendererVK.rc")

end_sources()

macro(add_common_libs)
	target_link_libraries(${THIS_PROJECT} PRIVATE Math)
	target_link_libraries(${THIS_PROJECT} PRIVATE freetype)
	target_link_libraries(${THIS_PROJECT} PRIVATE freeimage::FreeImage freeimage::FreeImagePlus)
	target_link_libraries(${THIS_PROJECT} PRIVATE Utils)
	target_link_libraries(${THIS_PROJECT} PRIVATE FxParser)
	add_sdl(${THIS_PROJECT})
endmacro()

macro(add_common_includes)
	target_include_directories(${THIS_PROJECT} PRIVATE ${Stb_INCLUDE_DIR})
endmacro()

if(OPTION_DX_RENDERER)
	message(STATUS "Using D3D11 Renderer Backend")
	enginemodule(RendererDX FORCE_SHARED_WIN PCH
							 "${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp" SOLUTION_FOLDER "Engine")
	add_common_libs()
	add_common_includes()

	find_package(D3D10 REQUIRED)
	target_compile_definitions(RendererDX PUBLIC "DX_RENDERER")
	target_link_libraries(${THIS_PROJECT} PRIVATE D3D10 d3dcompiler dxguid)
else()
	cryfilecontainer(RendererDX SOLUTION_FOLDER "Engine")
endif()
if(OPTION_VK_RENDERER)
	enginemodule(RendererVK FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER
							 "Engine")
	add_common_libs()
	add_common_includes()

	find_package(Vulkan REQUIRED)
	target_link_libraries(${THIS_PROJECT} PRIVATE Vulkan::Vulkan)
	target_compile_definitions(${THIS_PROJECT} PUBLIC VK_RENDERER)
	target_include_directories(${THIS_PROJECT} PUBLIC "Vulkan")
else()
	cryfilecontainer(RendererVK SOLUTION_FOLDER "Engine")
endif()

if(BISON_FOUND)
	bison_target(
		ShaderParser ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Parser.yy
		${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Parser.cpp
		# DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.hpp
	)
elseif(CYGWIN_INSTALL_PATH)
	message(STATUS "CYGWIN_INSTALL_PATH: ${CYGWIN_INSTALL_PATH}")
	cygwin_bison(${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Parser.yy
							 ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Parser.cpp)
endif()
if(FLEX_FOUND)
	flex_target(ShaderScanner ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Scanner.ll
							${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Scanner.cpp)
elseif(CYGWIN_INSTALL_PATH)
	message(STATUS "CYGWIN_INSTALL_PATH: ${CYGWIN_INSTALL_PATH}")
	cygwin_flex(${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Scanner.ll
							${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Scanner.cpp)
endif()
if(FLEX_FOUND AND BISON_FOUND)
	add_flex_bison_dependency(ShaderScanner ShaderParser)
endif()