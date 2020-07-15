cmake_minimum_required(VERSION 3.16)

project(ImGuiFileBrowser CXX)

add_library(${PROJECT_NAME}	INTERFACE)
target_include_directories(${PROJECT_NAME} 
	INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:BlackBox/GuiTools>
)

target_link_libraries(${PROJECT_NAME} INTERFACE imgui::imgui)
