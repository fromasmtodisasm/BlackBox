cmake_minimum_required(VERSION 3.0)
project(BlackBox CXX)

file(
  GLOB
  SOURCES
  src/*.cpp
    )
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
find_package(OpenGL REQUIRED)
#--------------------------------------------------
if (WIN32)
  find_library(GDI32_LIBRARY gdi32)
  if(GDI32_LIBRARY_FOUND)
    message(FATAL "gdi32 notfound")
  else()
    message("gdi32 founded")
    link_libraries(${GDI32_LIBRARY})
  endif()
endif()
#--------------------------------------------------
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})
  link_libraries(${GLEW_LIBRARIES})
else()
  message(FATAL "glew not found")
endif()
#--------------------------------------------------
find_package(GLM REQUIRED)
if (GLM_FOUND)
  include_directories(${GLM_INCLUDE_DIRS})
  link_libraries(${GLM_LIBRARIES})
  message("glm founded")
endif()
#--------------------------------------------------
include_directories(
  "${PROJECT_SOURCE_DIR}/include"
)
#--------------------------------------------------
###################################################
add_executable(${PROJECT_NAME}  ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC include)
