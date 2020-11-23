cmake_minimum_required(VERSION 3.14)

project(Steam)

add_library(Steam INTERFACE)

target_link_directories(Steam INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#find_library(steam_api64 steam_api64)
target_link_libraries(Steam INTERFACE lib/steam_api64)
target_include_directories(Steam INTERFACE include)

if (NOT PROJECT_INITED)
	file(COPY bin/steam_api64.dll DESTINATION ${OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
	message(STATUS "COPY bin/steam_api64.dll DESTINATION ${OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}")
endif()