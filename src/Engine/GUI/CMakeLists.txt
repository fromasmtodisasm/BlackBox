cmake_minimum_required(VERSION 3.15)
project(GUI CXX)
###################################################
add_library(GUI)

target_link_libraries(${PROJECT_NAME} PRIVATE Math)

target_include_directories(
${PROJECT_NAME}
	PRIVATE	"${CMAKE_BINARY_DIR}"
	PRIVATE	"${CMAKE_SOURCE_DIR}/include/"
)

target_sources(${PROJECT_NAME}
  PRIVATE ${ENGINE_DIR}/GUI/GUI.cpp
)
