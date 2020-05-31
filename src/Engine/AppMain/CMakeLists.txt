cmake_minimum_required(VERSION 3.10)
project(AppMain CXX)
###################################################
add_library(${PROJECT_NAME} OBJECT
	Main.cpp
)
target_link_libraries(AppMain PUBLIC  BlackBox)
target_link_libraries(AppMain PUBLIC  System)
target_link_libraries(${PROJECT_NAME} PUBLIC GUI)

add_private_headers()
set_target_properties(AppMain PROPERTIES FOLDER "Engine/Utils")
