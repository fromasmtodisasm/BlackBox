cmake_minimum_required(VERSION 3.3)
#project(glad)

set(GLAD_OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(GLAD_INCLUDE_DIRS "${GLAD_OUT_DIR}/include")
list(APPEND GLAD_SOURCES
"${GLAD_OUT_DIR}/src/glad.c"
"${GLAD_INCLUDE_DIRS}/glad/glad.h"
)

add_library(glad STATIC ${GLAD_SOURCES})

target_include_directories(glad INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  # <prefix>/include/mylib
)

target_include_directories(glad PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
