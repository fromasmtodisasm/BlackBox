# CMakeList.txt : CMake project for Common, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library (Common  "imgui_impl_dx11.cpp" "imgui_impl_sdl.cpp" "ClientServer.h" "main.cpp")

target_include_directories(
	Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Common PROPERTY CXX_STANDARD 20)
endif()

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Common PUBLIC imgui::imgui)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Common PUBLIC SDL2::SDL2 SDL2::SDL2main)

target_link_libraries(Common PUBLIC d3d11 Ws2_32)

# TODO: Add tests and install targets if needed.
