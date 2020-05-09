cmake_minimum_required(VERSION 3.10)
project(3DEngine CXX)
###################################################
add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  #Render folder
  PRIVATE ${PROJECT_SOURCE_DIR}/3DEngine.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Math)

include(Scene/CMakeLists.txt)
include(World/CMakeLists.txt)

add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)
