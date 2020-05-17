cmake_minimum_required(VERSION 3.3)

set(GLAD_OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(GLAD_INCLUDE_DIRS "${GLAD_OUT_DIR}/include")
list(APPEND GLAD_SOURCES
"${GLAD_OUT_DIR}/src/glad.c"
"${GLAD_INCLUDE_DIRS}/glad/glad.h"
)

add_library(glad STATIC ${GLAD_SOURCES})

target_include_directories(glad PUBLIC 
  $<BUILD_INTERFACE:${GLAD_INCLUDE_DIRS}>
  $<INSTALL_INTERFACE:include> 
)
set_target_properties(glad PROPERTIES FOLDER "Engine/ThirdParty")
install_this(glad)
