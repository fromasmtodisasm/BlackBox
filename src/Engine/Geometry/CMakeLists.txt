cmake_minimum_required(VERSION 3.10)
project(Geometry CXX)
###################################################
add_library(Geometry)

###################################################

target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGlBackend)

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Geometry.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/ObjLoader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Object.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Plane.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Primitives.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Geometry/Quad.cpp
)

add_private_headers()
