cmake_minimum_required(VERSION 3.10)
project(Network CXX)
###################################################
add_library(${PROJECT_NAME})

###################################################

#[[
if (DEFINED LINUX AND NOT DEFINED VCPKG_INSTALLER)
	find_package(sdl2-net CONFIG REQUIRED)
	find_package(SDL2 CONFIG REQUIRED)
else()
endif()
]]
add_SDL()
add_SDL_net()
target_link_libraries(${PROJECT_NAME} PRIVATE Math)

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/Network/Client.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Network/Network.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Network/Server.cpp
)

add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)
