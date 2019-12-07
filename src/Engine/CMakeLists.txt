cmake_minimum_required(VERSION 3.15)
project(BlackBox CXX)
###################################################

include(GenerateExportHeader)
include(FindFreetype)

###################################################
add_library(${PROJECT_NAME})
add_library(Common INTERFACE)
add_library(BB_Private INTERFACE)
###################################################
if (UNIX OR CYGWIN)
	set(UNIX_OR_CYGWIN ON)
	message(STATUS "UNIX_OR_CYGWIN")
	find_package(PkgConfig REQUIRED)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DLINUX64)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DLINUX)
else()

endif()
###################################################
#find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core)
###################################################
target_compile_features(BlackBox
PUBLIC
	cxx_strong_enums
PRIVATE
	cxx_lambdas
	cxx_range_for
	cxx_final
)

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
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DTEST)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DSINGLE_PASS)
endif()
find_package(OpenGL REQUIRED)
#--------------------------------------------------
target_include_directories(
BB_Private

	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/"
	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/Common"
	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/Interfaces/"
	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/Render/"
	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/Resources/"
	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/Utils/"
	INTERFACE "${SUBMODULES_DIR}/stb/"
)
#--------------------------------------------------
#--------------------------------------------------
###################################################
# Subs
###################################################
#add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/tinyxml2 tinyxml2)
add_subdirectory(${SOURCE_DIR}/Engine/Math Math)
add_subdirectory(${SOURCE_DIR}/Engine/Input Input)
add_subdirectory(${SOURCE_DIR}/Engine/Render Renderer)
add_subdirectory(${SOURCE_DIR}/Engine/ScriptSystem ScriptSystem)
add_subdirectory(${SOURCE_DIR}/Engine/Window Window)

target_link_libraries(${PROJECT_NAME} PUBLIC Input)
target_link_libraries(${PROJECT_NAME} PUBLIC Common)
target_link_libraries(${PROJECT_NAME} PUBLIC Math)
target_link_libraries(${PROJECT_NAME} PRIVATE Renderer)
target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)
###################################################
find_package(tinyxml2 CONFIG REQUIRED)
###################################################

###################################################
generate_export_header(${PROJECT_NAME}
	EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/Exports.hpp
)

find_package(Freetype REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FREETYPE_LIBRARIES})
#[[
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<INSTALL_INTERFACE:>  # <prefix>/include/mylib
  )
]]
###################################################
target_sources(
  ${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/GUI.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Scene.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptObjectConsole.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptObjectRenderer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/World.cpp
  #Common folder
  PRIVATE ${SOURCE_DIR}/Engine/Common/CmdLine.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Common/CmdLineArg.cpp
  #Geometry folder
  PRIVATE ${SOURCE_DIR}/Engine/Geometry.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/ObjLoader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Object.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Plane.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Primitives.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Quad.cpp

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

  #Resources folder
  PRIVATE ${SOURCE_DIR}/Engine/Resources/MaterialManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/ObjectManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/SceneManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/ShaderManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/TextureManager.cpp

  #System folder
  PRIVATE ${SOURCE_DIR}/Engine/System/Commands.cpp
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

if (GLAD_LOADER)
  target_sources(${PROJECT_NAME}
    PRIVATE ${SOURCE_DIR}/Engine/Render/GladLoader.cpp
  )
endif()

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
get_target_property(SOURCE_FILES ${PROJECT_NAME}  SOURCES)
source_group(
  TREE ${SOURCE_DIR}
  #PREFIX BlackBox
  FILES  ${SOURCE_FILES}
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
#target_link_libraries(${PROJECT_NAME} PUBLIC tinyxml2)
###################################################
target_link_libraries(${PROJECT_NAME} PRIVATE ScriptSystem)
target_link_libraries(${PROJECT_NAME} PRIVATE Window)
###################################################
#target_link_libraries(${PROJECT_NAME} ImGui)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGlBackend)
#target_link_libraries(${PROJECT_NAME} nvtt)
#target_include_directories(${PROJECT_NAME} PUBLIC include)
#/////////////////////////////////////////////////////////////////
target_include_directories (
Common
	INTERFACE	"${CMAKE_BINARY_DIR}"
	INTERFACE	"${CMAKE_SOURCE_DIR}/include/"
	)
target_link_libraries(Common INTERFACE tinyxml2::tinyxml2)
#/////////////////////////////////////////////////////////////////

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
