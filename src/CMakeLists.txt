cmake_minimum_required(VERSION 3.3)
project(BlackBox)

include(GenerateExportHeader)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Cmake/modules/")
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include)
set(BLACKBOX_INCLUDE_DIRS ${INCLUDE_DIRECTORIES}/BlackBox)

###################################################
#add_subdirectory(submodules/glfw)
if (GLAD_LOADER)
  #add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/glad glad)
  add_subdirectory(${CMAKE_SOURCE_DIR}/lib lib)
  add_definitions(-DGLAD_LOADER)
  add_definitions(-DGLAD_GLAPI_EXPORT_BUILD)
elseif(GLEW_LOADER)
  find_package(GLEW REQUIRED)
  add_definitions(-DGLEW_LOADER)
endif()
add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/tinyxml2 tinyxml2)
#add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/nvtt nvtt)
find_package(OpenGL REQUIRED)
#--------------------------------------------------
include_directories(
  "${SFML_SOURCE_DIR}/include"
  " ${CMAKE_SOURCE_DIR}/include/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/Game/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/Interfaces/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/Render/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/Resources/"
  " ${CMAKE_SOURCE_DIR}/include/BlackBox/Utils/"
)

#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#--------------------------------------------------
#find_package(Boost COMPONENTS system filesystem REQUIRED)
#...
#--------------------------------------------------
###################################################
add_library(${PROJECT_NAME})
generate_export_header(${PROJECT_NAME}
	EXPORT_FILE_NAME ${BLACKBOX_INCLUDE_DIRS}/Exports.hpp
)
if (USE_SFML)
  add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/SFML SFML)
  target_sources(
    ${PROJECT_NAME}
    PRIVATE ${SOURCE_DIR}/Engine/Window/SFMLWindow.cpp
    )
    add_definitions(-DSFML_WINDOW)
else()
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
  add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/glm glm)
  target_link_libraries(${PROJECT_NAME} glm)
  add_definitions(-DUSE_GLM)

endif()


target_sources(
  ${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/Console.cpp
  PRIVATE ${SOURCE_DIR}/Engine/GUI.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Log.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Scene.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptObjectConsole.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptObjectGame.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptObjectSystem.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System.cpp
  PRIVATE ${SOURCE_DIR}/Engine/World.cpp
  #Game folder
  PRIVATE ${SOURCE_DIR}/Engine/Game/Actions.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Game/Commands.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Game/Game.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Game/GameGUI.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Game/GameObject.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Game/GameTagPoints.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Game/Player.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Game/PlayerCommands.cpp
  #Geometry folder
  PRIVATE ${SOURCE_DIR}/Engine/Geometry.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/ObjLoader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Object.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Plane.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Primitives.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Quad.cpp

  #Input folder
  PRIVATE ${SOURCE_DIR}/Engine/Input/InputHandler.cpp
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
  PRIVATE ${SOURCE_DIR}/Engine/Render/Render.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ScreenShader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/Shader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ShadowMapShader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/ShadowMapTechnique.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Render/TechniqueManager.cpp
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
  #Utils folder
  PRIVATE ${SOURCE_DIR}/Engine/Utils/MusicList.cpp  
  PRIVATE ${SOURCE_DIR}/Engine/Utils/Utils.cpp
)

file(GLOB_RECURSE INCLUDE_FILES ${BLACKBOX_INCLUDE_DIRS}/*.hpp ${BLACKBOX_INCLUDE_DIRS}/*.h)
  
#--------------------------------------------------
if (LINUX OR UNIX OR APPLE)
	target_link_libraries(BlackBox ${CMAKE_DL_LIBS})
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
if (USE_SFML)
  target_link_libraries(${PROJECT_NAME} sfml-graphics)
  target_link_libraries(${PROJECT_NAME} sfml-audio)
else()
  target_link_libraries(${PROJECT_NAME} glfw)
endif()
if (GLAD_LOADER)
  target_link_libraries(${PROJECT_NAME} glad)
elseif(GLEW_LOADER)
  target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})
endif()
target_link_libraries(${PROJECT_NAME} tinyxml2)
#target_link_libraries(${PROJECT_NAME} ImGui)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
#target_link_libraries(${PROJECT_NAME} nvtt)
target_include_directories(${PROJECT_NAME} PUBLIC include)

#target_link_libraries(${PROJECT_NAME}
#  ${Boost_FILESYSTEM_LIBRARY}
#  ${Boost_SYSTEM_LIBRARY}
#)

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
