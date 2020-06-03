cmake_minimum_required(VERSION 3.10)
project(Window CXX)
###################################################
add_library(${PROJECT_NAME})
###################################################
if (OPTION_USE_SDL)
	message(STATUS "Using sdl for window managment")
	#################################################################
	#################################################################
  add_SDL()
	#################################################################
	#################################################################
	target_sources(
  	  ${PROJECT_NAME}
	  PRIVATE ${SOURCE_DIR}/Engine/Window/SDLWindow.cpp
	)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DSDL_WINDOW)
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

target_include_directories(
${PROJECT_NAME}
	PRIVATE $<TARGET_PROPERTY:OpenGlBackend,INTERFACE_INCLUDE_DIRECTORIES>
	#PRIVATE $<TARGET_PROPERTY:SDL2::SDL2,INTERFACE_INCLUDE_DIRECTORIES>
)

###################################################
target_link_libraries(${PROJECT_NAME} PUBLIC Math)
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGlBackend)

target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)
