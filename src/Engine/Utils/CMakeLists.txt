cmake_minimum_required(VERSION 3.10)
project(Utils CXX)
###################################################
add_library(${PROJECT_NAME})

###################################################

target_sources(${PROJECT_NAME}
  #Utils folder
  PRIVATE ${SOURCE_DIR}/Engine/Utils/MusicList.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Utils/Utils.cpp
)

add_private_headers()

target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)
