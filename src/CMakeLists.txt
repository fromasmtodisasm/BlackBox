cmake_minimum_required(VERSION 3.0)
project(BlackBox CXX)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Cmake/modules/")
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
###################################################
add_library(${PROJECT_NAME})
if (USE_SFML)
  target_sources(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/CSFMLWindow.cpp
    PRIVATE ${CMAKE_SOURCE_DIR}/include/CSFMLWindow.hpp
    )
else()
  target_sources(
    ${PROJECT_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/CGLFWWindow.cpp
    PRIVATE ${CMAKE_SOURCE_DIR}/include/CGLFWWindow.hpp
    )
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
endif()
target_sources(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/src/CGame.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/CInputHandler.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/CShader.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/CSystem.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/GameObject.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/Geometry.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/HackCamera.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/IndexBuffer.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/MatrixTransform.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/Object.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/ObjLoader.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/Plane.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/Primitives.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/Triangle.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/Utils.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/VertexArrayObject.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/VertexBuffer.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/World.cpp
  PRIVATE ${CMAKE_SOURCE_DIR}/src/CPlayer.cpp
)
target_sources(${PROJECT_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/include/CCamera.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/CGame.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/CInputHandler.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/common.h
  PRIVATE ${CMAKE_SOURCE_DIR}/include/CShader.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/CSystem.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/Cube.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/CWindow.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/GameObject.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/HackCamera.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/ICommand.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/IDrawable.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/IGame.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/IGeometry.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/IInput.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/IndexBuffer.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/InputHandler.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/IObject.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/IShaderManager.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/ISystem.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/IWindow.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/MatrixTranform.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/Object.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/ObjLoader.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/Opengl.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/pch.h
  PRIVATE ${CMAKE_SOURCE_DIR}/include/Plane.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/Primitives.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/Renderer.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/Tetrahedron.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/Triangle.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/Utils.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/VertexArrayObject.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/VertexBuffer.hpp
  PRIVATE ${CMAKE_SOURCE_DIR}/include/World.hpp
)
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
else()
  target_link_libraries(${PROJECT_NAME} glfw)
endif()
if (GLAD_LOADER)
  target_link_libraries(${PROJECT_NAME} glad)
elseif(GLEW_LOADER)
  target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})
endif()
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC include)


add_test(NAME mytest COMMAND ${PROJECT_NAME}_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

install(
  TARGETS  ${PROJECT_NAME}
  RUNTIME DESTINATION ${PROJECT_NAME}/bin
  LIBRARY DESTINATION ${PROJECT_NAME}/lib
  ARCHIVE DESTINATION ${PROJECT_NAME}/lib/static
)

install(
  DIRECTORY res DESTINATION ${PROJECT_NAME}
)
