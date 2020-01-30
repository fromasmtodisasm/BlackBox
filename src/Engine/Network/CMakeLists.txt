cmake_minimum_required(VERSION 3.10)
project(Network CXX)
###################################################
add_library(${PROJECT_NAME})

###################################################

find_package(sdl2-net CONFIG REQUIRED)
if (DEFINED LINUX)
	find_package(SDL2 CONFIG REQUIRED)
endif()
	find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_net)

target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)
target_link_libraries(${PROJECT_NAME} PRIVATE Common)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/Network/Client.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Network/Network.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Network/Server.cpp
)

add_private_headers()
