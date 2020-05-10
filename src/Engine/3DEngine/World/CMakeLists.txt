cmake_minimum_required(VERSION 3.10)
#project(World CXX)
###################################################

#target_link_libraries(${PROJECT_NAME} PRIVATE Scene)
target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/3DEngine/World/World.cpp
)

add_private_headers()
