cmake_minimum_required(VERSION 3.10)
project(Input CXX)
###################################################
add_library(${PROJECT_NAME})
###################################################
if (OPTION_USE_LINUXINPUT)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DUSE_LINUXINPUT)
endif()
message(STATUS "Using sdl for Input System")
#################################################################
add_SDL()
#################################################################
#target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE Utils)

target_include_directories(
${PROJECT_NAME}
	PRIVATE	"${CMAKE_BINARY_DIR}"
	PRIVATE	"${CMAKE_SOURCE_DIR}/include/"
	#PRIVATE $<TARGET_PROPERTY:SDL2,INTERFACE_INCLUDE_DIRECTORIES>
)

start_sources()
target_sources(${PROJECT_NAME}
	Input.cpp
	ActionMap.cpp
	ActionMapManager.cpp
	BaseInput.cpp
	InputActionMap.cpp
	InputCVars.cpp
	InputDevice.cpp
	InputHandler.cpp
	SDLInput.cpp
	SDLKeyboard.cpp
	SDLMouse.cpp
	SDLPad.cpp
)

add_private_headers()

target_precompile_headers(
Input
	PRIVATE
    "pch.hpp"
)
