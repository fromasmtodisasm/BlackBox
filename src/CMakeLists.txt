cmake_minimum_required(VERSION 3.3)
project(BlackBox CXX)
###################################################

include(GenerateExportHeader)
include(FindFreetype)

###################################################
if (UNIX OR CYGWIN)
	set(UNIX_OR_CYGWIN ON)
	message(STATUS "UNIX_OR_CYGWIN")
	find_package(PkgConfig REQUIRED)
  add_definitions(-DLINUX64)
  add_definitions(-DLINUX)
endif()
###################################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Tools/Cmake/modules/")
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(SUBMODULES_DIR ${CMAKE_SOURCE_DIR}/submodules)
set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include)
set(BLACKBOX_INCLUDE_DIRS ${INCLUDE_DIRECTORIES}/BlackBox)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###################################################
configure_file(${BLACKBOX_INCLUDE_DIRS}/Profiler/Config.h.in ${CMAKE_BINARY_DIR}/Config.h @ONLY)
###################################################

if (BB_TESET_ENABLE)
  add_definitions(-DTEST)
  add_definitions(-DSINGLE_PASS)
endif()
if (GLAD_LOADER)
  add_subdirectory(${CMAKE_SOURCE_DIR}/lib lib)
  add_definitions(-DGLAD_LOADER)
  add_definitions(-DGLAD_GLAPI_EXPORT_BUILD)
elseif(GLEW_LOADER)
  find_package(GLEW REQUIRED)
  add_definitions(-DGLEW_LOADER)
endif()
add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/tinyxml2 tinyxml2)
find_package(OpenGL REQUIRED)
#--------------------------------------------------
include_directories(
  " ${CMAKE_BINARY_DIR}"
  " ${SUBMODULES_DIR}/stb/"
  " ${CMAKE_SOURCE_DIR}/include/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/Common"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/Interfaces/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/Render/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/Resources/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/Utils/"
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#--------------------------------------------------
#find_package(Boost COMPONENTS system filesystem REQUIRED)
#...
#--------------------------------------------------
###################################################
add_library(${PROJECT_NAME})
generate_export_header(${PROJECT_NAME}
	EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/Exports.hpp
)
find_package(LUA REQUIRED)
if (LUA_FOUND)
	target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} PUBLIC ${LUA_LIBRARIES})
endif()

find_package(Freetype REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${FREETYPE_LIBRARIES})

if (USE_SDL)
	if (UNIX_OR_CYGWIN)
		#################################################################
		PKG_CHECK_MODULES(SDL REQUIRED SDL2)
		if (SDL_FOUND)
			target_link_libraries(${PROJECT_NAME} PUBLIC ${SDL2_LIBRARIES})
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
	add_definitions(-DSDL_WINDOW)
elseif (USE_SFML)

  find_package(SFML 2 REQUIRED audio graphics window system)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PUBLIC sfml-audio sfml-graphics sfml-window sfml-system)
  target_sources(
    ${PROJECT_NAME}
    PRIVATE ${SOURCE_DIR}/Engine/Window/SFMLWindow.cpp
    )
    add_definitions(-DSFML_WINDOW)
elseif(USE_GLFW)
  target_sources(
    ${PROJECT_NAME}
    PRIVATE ${SOURCE_DIR}/Engine/Window/GLFWWindow.cpp
    )
    add_definitions(-DGLFW_WINDOW)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
endif()

if (USE_GLM)
  #find_package(glm REQUIRED)
  if (NOT GLM_FOUND)
	message(STATUS "GLM NOTFOUND")
	add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/glm glm)
  else()
	  target_link_libraries(${PROJECT_NAME} PUBLIC glm)
	  include_directories(${GLM_INCLUDE_DIRS})
	  target_include_directories(${PROJECT_NAME} INTERFACE ${GLM_INCLUDE_DIR})
  endif()
  target_link_libraries(${PROJECT_NAME} PUBLIC glm)
  include_directories(${GLM_INCLUDE_DIRS})
  target_include_directories(${PROJECT_NAME} INTERFACE ${GLM_INCLUDE_DIR})
  add_definitions(-DUSE_GLM)
  add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

endif()
###################################################

set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<INSTALL_INTERFACE:>  # <prefix>/include/mylib
  )

###################################################
target_sources(
  ${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/GUI.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Scene.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptObjectConsole.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptObjectRenderer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptObjectScript.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptObjectSystem.cpp
  PRIVATE ${SOURCE_DIR}/Engine/World.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Common/CmdLine.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Common/CmdLineArg.cpp
  #Geometry folder
  PRIVATE ${SOURCE_DIR}/Engine/Geometry.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/ObjLoader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Object.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Plane.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Primitives.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Quad.cpp

  #Input folder
  PRIVATE ${SOURCE_DIR}/Engine/Input/BaseInput.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Input/InputCVars.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Input/InputDevice.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Input/InputHandler.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Input/SDLInput.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Input/SDLKeyboard.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Input/SDLMouse.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Input/SDLPad.cpp
  #Network folder
  PRIVATE ${SOURCE_DIR}/Engine/Network/Client.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Network/Network.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Network/Server.cpp

  #Profiler folder
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/Drawer2D.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/HP_Timer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/Profiler.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/Tgaloader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/Thread.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/Utils.cpp

  #Render folder
  PRIVATE ${SOURCE_DIR}/Engine/Render/BaseShader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/BaseTexture.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Camera.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/FrameBufferObject.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/FreeTypeFont.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/GladLoader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/HdrTechnique.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/IndexBuffer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Light.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Material.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/OpenglDebug.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Particles.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Pipeline.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Postprocessor.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ReflectShader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/GLRenderer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/BaseRenderer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ScreenShader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Shader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ShadowMapShader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ShadowMapTechnique.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/TechniqueManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Terrain.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Texture.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/TextureCube.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/VertexArrayObject.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/VertexBuffer.cpp

  #Resources folder
  PRIVATE ${SOURCE_DIR}/Engine/Resources/MaterialManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/ObjectManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/SceneManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/ShaderManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/TextureManager.cpp
  #ScriptSystem folder
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptBinding.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/FunctionHandler.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptObject.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptSystem.cpp

  #System folder
  PRIVATE ${SOURCE_DIR}/Engine/System/Console.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/Linux/Runtime.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/Log.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/System.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/SystemEventDispatcher.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/Timer.cpp
  #Utils folder
  PRIVATE ${SOURCE_DIR}/Engine/Utils/MusicList.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Utils/Utils.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Utils/Cursor.cpp
)

if (UNIX_OR_CYGWIN)
  target_sources(
    ${PROJECT_NAME}
    PRIVATE ${SOURCE_DIR}/Engine/System/Linux/Runtime.cpp
    )

endif()
###################################################
file(GLOB_RECURSE INCLUDE_FILES ${BLACKBOX_INCLUDE_DIRS}/*.hpp ${BLACKBOX_INCLUDE_DIRS}/*.h)

#--------------------------------------------------
if (LINUX OR UNIX OR APPLE)
	target_link_libraries(BlackBox PUBLIC ${CMAKE_DL_LIBS})
endif()
#--------------------------------------------------
if (GUI)
	#IMGUI
	#if (BUILD_SHARED_LIBS)
	#  add_library(ImGui SHARED)
	#  message(STATUS "build shared libs!")
	#else()
	#  add_library(ImGui STATIC)
	#  message(STATUS "build static libs!")
	#endif()
	set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/submodules/imgui)
	set(IMGUI-SFML_DIR ${CMAKE_SOURCE_DIR}/submodules/imgui-sfml)

	target_include_directories(
	  ${PROJECT_NAME} PUBLIC ${IMGUI_DIR}
	)
	target_include_directories(
	  ${PROJECT_NAME} PUBLIC ${IMGUI-SFML_DIR}
	)
	set(
	  IMGUI_SOURCES
	  ${IMGUI_DIR}/imgui.cpp
	  ${IMGUI_DIR}/imgui_widgets.cpp
	  ${IMGUI_DIR}/imgui_draw.cpp
	  ${IMGUI-SFML_DIR}/imgui-SFML.cpp
	  ${IMGUI_DIR}/imgui_demo.cpp
	  )
endif(GUI)
#--------------------------------------------------
if (GLEW_LOADER)
  target_sources(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/Engine/Render/GlewLoader.cpp
  )
elseif(GLAD_LOADER)
  target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCE_DIR}/Engine/Render/GladLoader.cpp
  )
endif()
#--------------------------------------------------
get_target_property(SOURCE_FILES ${PROJECT_NAME}  SOURCES)
source_group(
  TREE ${SOURCE_DIR}
  PREFIX BlackBox
  FILES
  ${SOURCE_FILES}
  )
target_sources(${PROJECT_NAME}
  PRIVATE
  ${INCLUDE_FILES}
  )
source_group(
  TREE ${CMAKE_SOURCE_DIR}/include/BlackBox
  PREFIX BlackBox\\Inc
  FILES
  ${INCLUDE_FILES}
  )
target_sources(${PROJECT_NAME}
  PRIVATE
  ${IMGUI_SOURCES}
  )
source_group(
  BlackBox\\Engine\\Gui
  #TREE ${CMAKE_SOURCE_DIR}/submodules
  #PREFIX BlackBox\\Engine\\Gui
  FILES
  ${IMGUI_SOURCES}
  )
#--------------------------------------------------
if (GLAD_LOADER)
  target_link_libraries(${PROJECT_NAME} PUBLIC glad)
elseif(GLEW_LOADER)
  target_link_libraries(${PROJECT_NAME} PUBLIC ${GLEW_LIBRARY})
endif()
target_link_libraries(${PROJECT_NAME} PUBLIC tinyxml2)
#target_link_libraries(${PROJECT_NAME} ImGui)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
#target_link_libraries(${PROJECT_NAME} nvtt)
target_include_directories(${PROJECT_NAME} PUBLIC include)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:BlackBox>/dlls)

install(
  TARGETS  ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  #ARCHIVE DESTINATION lib/static
)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION ${PROJECT_NAME}
)
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/media DESTINATION ${PROJECT_NAME}
)

#install(FILES
# BlackBoxExport.h
# ${PROJECT_BINARY_DIR}/blackbox_export.h DESTINATION ${BLACKBOX_INCLUDE_DIRS}
#)
