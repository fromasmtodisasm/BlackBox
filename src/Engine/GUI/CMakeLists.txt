cmake_minimum_required(VERSION 3.14)
project(GUI CXX)
###################################################
add_library(GUI)

target_link_libraries(${PROJECT_NAME} PRIVATE Math)

target_include_directories(
${PROJECT_NAME}
	PRIVATE	"${CMAKE_BINARY_DIR}"
	PRIVATE	"${CMAKE_SOURCE_DIR}/"
)

target_sources(${PROJECT_NAME}
  PRIVATE ${ENGINE_DIR}/GUI/GUI.cpp
  PRIVATE ${ENGINE_DIR}/GUI/ImGuiManager.cpp
  PRIVATE ${ENGINE_DIR}/GUI/imgui_impl_opengl3.cpp
  PRIVATE ${ENGINE_DIR}/GUI/imgui_impl_sdl.cpp
  PRIVATE ${ENGINE_DIR}/GUI/imgui_demo.cpp
)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
add_compile_definitions(GUI)

add_private_headers()
