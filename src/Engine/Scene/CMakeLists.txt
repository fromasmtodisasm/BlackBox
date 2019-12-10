cmake_minimum_required(VERSION 3.15)
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
)

add_headers()
