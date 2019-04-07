cmake_minimum_required(VERSION 3.0)
project(BlackBox CXX)

enable_testing()

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
add_subdirectory(submodules/glm)
add_subdirectory(submodules/SFML)
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
  "${PROJECT_SOURCE_DIR}/include"
)
#--------------------------------------------------
###################################################

add_library(${PROJECT_NAME}  ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} glad)
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

