cmake_minimum_required(VERSION 3.0)
project(BlackBox CXX)

if (CYGWIN)
  #FOR GLFW COMPILATION
  set (WIN32 1)
  find_library(GDI32_LIBRARY gdi32)
endif()

set(SOURCES
  src/Main.cpp
  src/CWindow.cpp
    )

set(HEADERS
  include/CWindow.hpp
  include/IWindow.hpp
    )

set(CMAKE_MODULE_PATH ./cmake/modules)

#GLFW

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS ON)
set(CMAKE_TOOLCHAIN_FILE=CMake/x86_64-w64-mingw32.cmake)
add_subdirectory(submodules/glfw)
find_package(OpenGL REQUIRED)
#==============================================

include_directories(
        ${OPENGL_INCLUDE_DIR}
        ${GLFW_INCLUDE_DIRS}
        "${PROJECT_SOURCE_DIR}/include"
)

#add_definitions(-DGLFW_DLL)
add_executable(${PROJECT_NAME}  ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${OPENGL_glu_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC include)
if (WIN32)
  target_link_libraries(${PROJECT_NAME} ${GDI32_LIBRARY})
endif()
