cmake_minimum_required(VERSION 3.10)
project(3DEngine CXX)
###################################################
add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  #Render folder
  PRIVATE ${PROJECT_SOURCE_DIR}/3DEngine.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/MaterialManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/ObjectManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/SceneManager.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Main.cpp


  PRIVATE ${PROJECT_SOURCE_DIR}/Geometry/Geometry.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Geometry/ObjLoader.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Geometry/StatObj.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Geometry/Plane.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Geometry/Primitives.cpp
  PRIVATE ${PROJECT_SOURCE_DIR}/Geometry/Quad.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Math)
#add_package(assimp)
#target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)

include(Scene/CMakeLists.txt)
include(World/CMakeLists.txt)

add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)
