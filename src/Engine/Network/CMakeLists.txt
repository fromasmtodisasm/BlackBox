cmake_minimum_required(VERSION 3.14)
project(Network CXX)
###################################################
add_library(${PROJECT_NAME})

###################################################

target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)
target_link_libraries(${PROJECT_NAME} PRIVATE Common)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/Network/Client.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Network/Network.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Network/Server.cpp
)

add_private_headers()
