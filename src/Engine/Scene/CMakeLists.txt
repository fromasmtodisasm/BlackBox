cmake_minimum_required(VERSION 3.14)
project(Scene CXX)
###################################################
add_library(Scene)

###################################################

target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)
target_link_libraries(${PROJECT_NAME} PRIVATE Common)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGlBackend)

target_sources(${PROJECT_NAME}
  PRIVATE ${ENGINE_DIR}/Scene/Scene.cpp
  PRIVATE ${ENGINE_DIR}/Scene/SceneSerialization.cpp
  PRIVATE ${ENGINE_DIR}/Scene/XmlSerialization.cpp
)

add_private_headers()
