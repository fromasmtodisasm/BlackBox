cmake_minimum_required(VERSION 3.10)
set(OPTION_USE_GUI ON)
project(Configurator CXX)
###################################################
add_executable(${PROJECT_NAME} 
	MainWindow.cpp
    "Resource.rc"
)
target_link_libraries(Configurator PRIVATE  AppMain)
target_link_libraries(Configurator PRIVATE  Renderer)

add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)

set_property(TARGET Configurator PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set_target_properties(Configurator PROPERTIES FOLDER "Engine/Utils")
