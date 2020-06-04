cmake_minimum_required(VERSION 3.10)
project(Editor CXX)
###################################################
add_executable(${PROJECT_NAME} 
	Main.cpp
	MainWindow.cpp
    "Resource.rc"
)
target_link_libraries(Editor PRIVATE  BlackBox)
target_link_libraries(Editor PRIVATE  System)
add_dependencies(Editor Game)
if (OPTION_USE_GUI)
	target_link_libraries(${PROJECT_NAME} PRIVATE GUI)
endif()

target_sources(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)

set_property(TARGET Editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set_target_properties(Editor PROPERTIES FOLDER "Engine/Editor")
