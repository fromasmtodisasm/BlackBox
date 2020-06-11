cmake_minimum_required(VERSION 3.10)
project(EntitySystem CXX)
###################################################
add_library(EntitySystem)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE Utils)

target_sources(${PROJECT_NAME}
  PRIVATE ${ENGINE_DIR}/EntitySystem/EntitySystem.cpp
  PRIVATE ${ENGINE_DIR}/EntitySystem/Entity.cpp
)

get_target_property(SOURCE_FILES EntitySystem SOURCES )

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}
	PREFIX "Source Files"
	FILES
	${SOURCE_FILES}
)

target_link_libraries(EntitySystem PRIVATE System)

add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)
