cmake_minimum_required(VERSION 3.3)
project(BlackBox)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Cmake/modules/")
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include)
set(BLACKBOX_INCLUDE_DIRS ${INCLUDE_DIRECTORIES}/BlackBox)
###################################################
#add_subdirectory(submodules/glfw)
if (GLAD_LOADER)
  add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/glad glad)
  add_definitions(-DGLAD_LOADER)
elseif(GLEW_LOADER)
  find_package(GLEW REQUIRED)
  add_definitions(-DGLEW_LOADER)
endif()
add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/glm glm)
add_subdirectory(${CMAKE_SOURCE_DIR}/submodules/SFML SFML)
find_package(OpenGL REQUIRED)
#--------------------------------------------------
if (WIN32)
  find_library(GDI32_LIBRARY gdi32)
  if(GDI32_LIBRARY_FOUND)
    message(STATUS "gdi32 found")
    link_libraries(${GDI32_LIBRARY})
  else()
    message(FAILED "GDI32 notfound")
  endif()
endif()
#--------------------------------------------------
include_directories(
  "${SFML_SOURCE_DIR}/include"
  " ${CMAKE_SOURCE_DIR}/include"
)
#--------------------------------------------------
#find_package(Boost COMPONENTS system filesystem REQUIRED)
#...
#--------------------------------------------------
###################################################
add_library(${PROJECT_NAME})
if (USE_SFML)
  target_sources(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/CSFMLWindow.cpp
    PRIVATE ${BLACKBOX_INCLUDE_DIRS}/CSFMLWindow.hpp
    )
    add_definitions(-DSFML_WINDOW)
else()
  target_sources(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/CGLFWWindow.cpp
    PRIVATE ${BLACKBOX_INCLUDE_DIRS}/CGLFWWindow.hpp
    )
    add_definitions(-DGLFW_WINDOW)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
endif()
target_sources(
  ${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/CGame.cpp
  PRIVATE ${SOURCE_DIR}/CInputHandler.cpp
  PRIVATE ${SOURCE_DIR}/CShader.cpp
  PRIVATE ${SOURCE_DIR}/CEngine.cpp
  PRIVATE ${SOURCE_DIR}/GameObject.cpp
  PRIVATE ${SOURCE_DIR}/Geometry.cpp
  PRIVATE ${SOURCE_DIR}/GUI.cpp
  PRIVATE ${SOURCE_DIR}/CCamera.cpp
  PRIVATE ${SOURCE_DIR}/IndexBuffer.cpp
  PRIVATE ${SOURCE_DIR}/MatrixTransform.cpp
  PRIVATE ${SOURCE_DIR}/Object.cpp
  PRIVATE ${SOURCE_DIR}/ObjLoader.cpp
  PRIVATE ${SOURCE_DIR}/Plane.cpp
  PRIVATE ${SOURCE_DIR}/Primitives.cpp
  PRIVATE ${SOURCE_DIR}/Triangle.cpp
  PRIVATE ${SOURCE_DIR}/Utils.cpp
  PRIVATE ${SOURCE_DIR}/VertexArrayObject.cpp
  PRIVATE ${SOURCE_DIR}/VertexBuffer.cpp
  PRIVATE ${SOURCE_DIR}/World.cpp
  PRIVATE ${SOURCE_DIR}/CPlayer.cpp
  PRIVATE ${SOURCE_DIR}/Texture.cpp
)
target_sources(${PROJECT_NAME}
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/CCamera.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/Texture.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/CGame.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/CInputHandler.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/common.h
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/CShader.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/CEngine.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/Cube.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/CWindow.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/GameObject.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/GUI.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/ICommand.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/IDrawable.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/IGame.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/IGeometry.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/IInput.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/IndexBuffer.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/InputHandler.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/IObject.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/IShaderManager.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/IEngine.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/IWindow.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/MatrixTranform.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/MusicList.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/Object.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/ObjLoader.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/Opengl.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/pch.h
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/Plane.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/Primitives.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/Renderer.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/Tetrahedron.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/Triangle.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/Utils.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/VertexArrayObject.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/VertexBuffer.hpp
  PRIVATE ${BLACKBOX_INCLUDE_DIRS}/World.hpp
)
#--------------------------------------------------
if (LINUX OR UNIX OR APPLE)
	target_link_libraries(BlackBox ${CMAKE_DL_LIBS})
endif()
#--------------------------------------------------
#IMGUI
add_library(ImGui STATIC)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/submodules/imgui)
set(IMGUI-SFML_DIR ${CMAKE_SOURCE_DIR}/submodules/imgui-sfml)

target_include_directories(
  ImGui PUBLIC ${IMGUI_DIR}
)
target_include_directories(
  ImGui PUBLIC ${IMGUI-SFML_DIR}
)
target_sources(
  ImGui
  PUBLIC ${IMGUI_DIR}/imgui.cpp
  PUBLIC ${IMGUI_DIR}/imgui_widgets.cpp
  PUBLIC ${IMGUI_DIR}/imgui_draw.cpp
  PUBLIC ${IMGUI-SFML_DIR}/imgui-SFML.cpp
  PUBLIC ${IMGUI_DIR}/imgui_demo.cpp
  )
#--------------------------------------------------
if (GLEW_LOADER)
  target_sources(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/GlewLoader.cpp
  )
elseif(GLAD_LOADER)
  target_sources(${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/GladLoader.cpp
  )
endif()

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
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} ImGui)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC include)

#target_link_libraries(${PROJECT_NAME}
#  ${Boost_FILESYSTEM_LIBRARY}
#  ${Boost_SYSTEM_LIBRARY}
#)

install(
  TARGETS  ${PROJECT_NAME}
  RUNTIME DESTINATION ${PROJECT_NAME}/bin
  LIBRARY DESTINATION ${PROJECT_NAME}/lib
  ARCHIVE DESTINATION ${PROJECT_NAME}/lib/static
)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION ${PROJECT_NAME}
)
