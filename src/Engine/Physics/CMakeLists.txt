cmake_minimum_required(VERSION 3.10)
project(Physics CXX)
###################################################
add_library(${PROJECT_NAME})
###################################################
target_include_directories(
${PROJECT_NAME}
	PUBLIC	"${CMAKE_BINARY_DIR}"
	PUBLIC	"${CMAKE_SOURCE_DIR}/include/"
)

target_sources(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/Physics.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Math)

add_private_headers()
