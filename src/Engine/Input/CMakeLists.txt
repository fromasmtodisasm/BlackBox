cmake_minimum_required(VERSION 3.10)
project(Input CXX)
###################################################
add_library(Input)

###################################################
if (OPTION_USE_SDL)
	message(STATUS "Using sdl for window managment")
	#################################################################
	#add_package(SDL2 CONFIG)
	find_package(SDL2 CONFIG REQUIRED)
	if (DEFINED LINUX)
		target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static)
	else()
		target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
	endif()
	#################################################################
	#add_package(SDL2-net)
	find_package(sdl2-net CONFIG REQUIRED)
	#target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_net)
	target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_net)
	#################################################################
elseif (USE_SFML)
	find_package(SFML 2 REQUIRED audio graphics window system)
	include_directories(${SFML_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} PUBLIC sfml-audio sfml-graphics sfml-window sfml-system)
	target_sources(
	${PROJECT_NAME}
	PRIVATE ${SOURCE_DIR}/Engine/Window/SFMLWindow.cpp
	)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DSFML_WINDOW)
elseif(USE_GLFW)
	target_sources(
		${PROJECT_NAME}
		PRIVATE ${SOURCE_DIR}/Engine/Window/GLFWWindow.cpp
	)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DGLFW_WINDOW)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
endif()

#target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)

target_include_directories(
${PROJECT_NAME}
	PRIVATE	"${CMAKE_BINARY_DIR}"
	PRIVATE	"${CMAKE_SOURCE_DIR}/include/"
	#PRIVATE $<TARGET_PROPERTY:SDL2,INTERFACE_INCLUDE_DIRECTORIES>
)

target_sources(${PROJECT_NAME}
	PRIVATE ${SOURCE_DIR}/Engine/Input/ActionMap.cpp
	PRIVATE ${SOURCE_DIR}/Engine/Input/ActionMapManager.cpp
	PRIVATE ${SOURCE_DIR}/Engine/Input/BaseInput.cpp
	PRIVATE ${SOURCE_DIR}/Engine/Input/InputActionMap.cpp
	PRIVATE ${SOURCE_DIR}/Engine/Input/InputCVars.cpp
	PRIVATE ${SOURCE_DIR}/Engine/Input/InputDevice.cpp
	PRIVATE ${SOURCE_DIR}/Engine/Input/InputHandler.cpp
	PRIVATE ${SOURCE_DIR}/Engine/Input/SDLInput.cpp
	PRIVATE ${SOURCE_DIR}/Engine/Input/SDLKeyboard.cpp
	PRIVATE ${SOURCE_DIR}/Engine/Input/SDLMouse.cpp
	PRIVATE ${SOURCE_DIR}/Engine/Input/SDLPad.cpp
)

add_private_headers()
