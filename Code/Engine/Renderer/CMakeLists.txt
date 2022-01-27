add_package(stb)
add_package(freetype)
add_package(freeimage)

add_subdirectory(Shaders)

start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	RenderD3D11
	RendererGL
	RendererVK
	SOURCE_GROUP "Source Files"
		"AuxRenderer.cpp"
		"AuxRendererNull.cpp"
		"BaseRenderer.cpp"
		"FreeTypeFont.cpp"
		"Shaders/FxParser.cpp"
	SOURCE_GROUP "Common"
		"Common/CommonRender.h"
		"Common/CommonRender.cpp"
		"Common/CryNameR.h"
		"Common/ResourceManager.hpp"
		"Common/ResourceManager.cpp"
	SOURCE_GROUP "GraphicsPipeline"
		"GraphicsPipeline/StandardGraphicsPipeline.cpp"
		"GraphicsPipeline/StandardGraphicsPipeline.h"
	SOURCE_GROUP "GraphicsPipeline\\\\Common"
		"GraphicsPipeline/Common/GraphicsPipeline.cpp"
		"GraphicsPipeline/Common/GraphicsPipeline.h"
		"GraphicsPipeline/Common/GraphicsPipelineStage.cpp"
		"GraphicsPipeline/Common/GraphicsPipelineStage.h"
)

add_sources(
	"NoUberFile"
	PROJECTS
	RenderD3D11
	SOURCE_GROUP "D3D"
		"D3D/BufferManager.cpp"
		"D3D/CMakeLists.txt"
		"D3D/DDSTextureLoader11.cpp"
		"D3D/DDSTextureLoader11.h"
		"D3D/Device.cpp"
		"D3D/Device.hpp"
		"D3D/RenderThread.cpp"
		"D3D/RenderThread.h"
		"D3D/Renderer.cpp"
		"D3D/Renderer.h"
		"D3D/Shader.cpp"
		"D3D/Shader.hpp"
		"D3D/SwapChain.cpp"
		"D3D/SwapChain.hpp"

	SOURCE_GROUP "DeviceManager"
		"DeviceManager/DeviceObjects.h"
		"DeviceManager/Deviceobjects.cpp"
		"DeviceManager/DeviceResources.cpp"
		"DeviceManager/DeviceResources.hpp"
	
)

add_sources(
	"NoUberFile"
	PROJECTS
	RendererVK
	SOURCE_GROUP "Vulkan"
		"Vulkan/Renderer.cpp"
		"Vulkan/Shader.cpp"
		"Vulkan/Renderer.h"
)
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
	EngineModule(RenderD3D11 FORCE_SHARED_WIN PCH
							 "${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp" SOLUTION_FOLDER "Engine")
	add_common_libs()
	add_common_includes()

	target_compile_definitions(RenderD3D11 PUBLIC "DX_RENDERER")
	target_link_libraries(${THIS_PROJECT} PRIVATE dxguid d3d11 d3d10 d3d9 d3dcompiler DXGI.lib)
	add_library(Renderer ALIAS RenderD3D11)
else()
	cryfilecontainer(RenderD3D11 SOLUTION_FOLDER "Engine")
endif()
if(OPTION_VK_RENDERER)
	EngineModule(RendererVK FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER
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

