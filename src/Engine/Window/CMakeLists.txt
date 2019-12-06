cmake_minimum_required(VERSION 3.15)
project(Window CXX)
###################################################
add_library(${PROJECT_NAME})
###################################################
target_include_directories(
${PROJECT_NAME}
	PUBLIC	"${CMAKE_BINARY_DIR}"
	PUBLIC	"${CMAKE_SOURCE_DIR}/include/"
	#PRIVATE $<TARGET_PROPERTY:OpenGLBackend,INTERFACE_INCLUDE_DIRECTORIES>
)

if (USE_SDL)
	message(STATUS "Using sdl for window managment")
	if (UNIX_OR_CYGWIN)
		#################################################################
		PKG_CHECK_MODULES(SDL SDL2)
		if (SDL_FOUND)
			target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
    else()
      find_package(SDL2 CONFIG REQUIRED)
      if (SDL2_FOUND)
        target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)
      else()
        message(FATAL_ERROR "Could not found SDL2")
      endif()
      
		endif()
		#################################################################
		PKG_CHECK_MODULES(SDLNET REQUIRED SDL2_net)
		if (SDLNET_FOUND)
			target_link_libraries(${PROJECT_NAME} PUBLIC ${SDLNET_LIBRARIES})
		endif()
		
	else()
		#################################################################
		find_package(SDL2 REQUIRED)
		if (SDL_FOUND)
			target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)
		endif()
		#################################################################
		find_package(SDL2-net REQUIRED)
		if (SDL2-net_FOUND)
			target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_net)
		endif()
	endif()
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

###################################################
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} INTERFACE OpenGLBackend)
