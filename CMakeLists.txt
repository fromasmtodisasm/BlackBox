cmake_minimum_required(VERSION 3.0)
project(BlackBox CXX)

enable_testing()

file(
  GLOB 
  HEADERS 
  include/*.hpp
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Cmake/modules/")
###################################################
#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
#--------------------------------------------------
add_subdirectory(submodules/glfw)
if (GLAD_LOADER)
  add_subdirectory(submodules/glad)
elseif(GLEW_LOADER)
  find_package(GLEW REQUIRED)
endif()
add_subdirectory(submodules/glm)
add_subdirectory(submodules/SFML)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
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
  "${PROJECT_SOURCE_DIR}/include"
)
#--------------------------------------------------
###################################################
add_definitions(-DGLEW_LOADER)
add_library(${PROJECT_NAME}) 
if (USE_SFML)
  target_sources(
    ${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/src/CSFMLWindow.cpp
    PUBLIC ${CMAKE_SOURCE_DIR}/include/CSFMLWindow.hpp
    )
else()
  target_sources(
    ${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/src/CGLFWWindow.cpp
    PUBLIC ${CMAKE_SOURCE_DIR}/include/CGLFWWindow.hpp
    )
endif()
target_sources(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_SOURCE_DIR}/src/CGame.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/CInputHandler.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/CShader.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/CSystem.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/GameObject.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/Geometry.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/HackCamera.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/IndexBuffer.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/MatrixTransform.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/Object.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/ObjLoader.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/Plane.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/Primitives.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/Triangle.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/Utils.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/VertexArrayObject.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/VertexBuffer.cpp
  PUBLIC ${CMAKE_SOURCE_DIR}/src/World.cpp
)
target_sources(${PROJECT_NAME}
	PUBLIC ${CMAKE_SOURCE_DIR}/include/CCamera.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/CGame.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/CInputHandler.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/common.h
	PUBLIC ${CMAKE_SOURCE_DIR}/include/CShader.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/CSystem.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/Cube.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/CWindow.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/GameObject.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/HackCamera.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/ICommand.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/IDrawable.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/IGame.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/IGeometry.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/IInput.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/IndexBuffer.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/InputHandler.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/IObject.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/IShaderManager.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/ISystem.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/IWindow.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/MatrixTranform.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/Object.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/ObjLoader.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/Opengl.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/pch.h
	PUBLIC ${CMAKE_SOURCE_DIR}/include/Plane.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/Primitives.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/Renderer.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/Tetrahedron.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/Triangle.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/Utils.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/VertexArrayObject.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/VertexBuffer.hpp
	PUBLIC ${CMAKE_SOURCE_DIR}/include/World.hpp
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


target_link_libraries(${PROJECT_NAME} glfw)
if (GLAD_LOADER)
  target_link_libraries(${PROJECT_NAME} glad)
elseif(GLEW_LOADER)
  target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})
endif()
target_link_libraries(${PROJECT_NAME} glm)
target_link_libraries(${PROJECT_NAME} sfml-graphics)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC include)

add_executable(${PROJECT_NAME}_test  tests/Main.cpp)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
if(WIN32)
  if (MSVC)
    #SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows")
    #set_target_properties(${PROJECT_NAME}_test PROPERTIES CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows)
  endif()
  if (MINGW)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--subsystem,windows")
  endif()
endif(WIN32)


add_test(NAME mytest COMMAND ${PROJECT_NAME}_test WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

install(
  TARGETS ${PROJECT_NAME}_test ${PROJECT_NAME}  
  RUNTIME DESTINATION ${PROJECT_NAME}/bin
  LIBRARY DESTINATION ${PROJECT_NAME}/lib
  ARCHIVE DESTINATION ${PROJECT_NAME}/lib/static
)

install(
  DIRECTORY res DESTINATION ${PROJECT_NAME}
)

