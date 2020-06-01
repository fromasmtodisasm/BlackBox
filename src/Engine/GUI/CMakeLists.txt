cmake_minimum_required(VERSION 3.10)
project(GUI CXX)
###################################################
add_library(GUI SHARED)

target_link_libraries(${PROJECT_NAME} PRIVATE Math)

target_include_directories(
${PROJECT_NAME}
	PRIVATE	"${CMAKE_BINARY_DIR}"
	PRIVATE	"${CMAKE_SOURCE_DIR}/"
  PUBLIC	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
)

target_sources(${PROJECT_NAME}
  PRIVATE ${ENGINE_DIR}/GUI/GUI.cpp
  PRIVATE ${ENGINE_DIR}/GUI/ImGuiManager.cpp
  PRIVATE ${ENGINE_DIR}/GUI/ImGuiInput.cpp
  PRIVATE ${ENGINE_DIR}/GUI/ImGuiRenderer.cpp

  PRIVATE ${ENGINE_DIR}/GUI/imgui_demo.cpp

  PRIVATE ${ENGINE_DIR}/GUI/imgui.cpp
  PRIVATE ${ENGINE_DIR}/GUI/imgui_draw.cpp
  PRIVATE ${ENGINE_DIR}/GUI/imgui_widgets.cpp
)

#add_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Renderer)
target_compile_definitions(GUI PRIVATE IMGUI_EXPORTS)
#target_compile_definitions(${PROJECT_NAME} PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_CUSTOM <BlackBox/Renderer/OpenGL/Core.hpp>)
add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
	<BlackBox/Core/Platform/Platform.hpp>
)
