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
add_subdirectory(submodules/glad)
find_package(OpenGL REQUIRED)
#--------------------------------------------------
if (NOT MSVC)
  find_library(GDI32_LIBRARY gdi32)
  if(GDI32_LIBRARY_FOUND)
    message(FATAL "gdi32 notfound")
  else()
    message(STATUS "GDI32 founded")
    link_libraries(${GDI32_LIBRARY})
  endif()
endif()
#--------------------------------------------------
find_package(GLM)
if (GLM_FOUND)
  include_directories(${GLM_INCLUDE_DIRS})
  link_libraries(${GLM_LIBRARIES})
  message(STATUS "GLM founded")
else()
  add_subdirectory(submodules/glm)
endif()
#--------------------------------------------------
include_directories(
  "${PROJECT_SOURCE_DIR}/include"
)
#--------------------------------------------------
###################################################
add_library(${PROJECT_NAME}  ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} ${GLM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})
target_include_directories(${PROJECT_NAME} PUBLIC include)

add_executable(${PROJECT_NAME}_test  tests/Main.cpp)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})

install(
  TARGETS ${PROJECT_NAME}_test ${PROJECT_NAME}  
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
)

install(
  DIRECTORY res DESTINATION ${CMAKE_INSTALL_PREFIX}
)

