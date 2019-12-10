cmake_minimum_required(VERSION 3.15)
project(World CXX)
###################################################
add_library(${PROJECT_NAME})

###################################################

target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)
target_link_libraries(${PROJECT_NAME} PRIVATE Common)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGlBackend)

target_link_libraries(${PROJECT_NAME} PRIVATE Scene)
target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/World/World.cpp
)

add_headers()
