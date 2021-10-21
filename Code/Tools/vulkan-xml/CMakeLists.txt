cmake_minimum_required(VERSION 3.10)

project(vulkan-xml)

add_executable(vulkan-xml main.cpp)

ADD_PACKAGE(pugixml)
set_target_properties(pugixml PROPERTIES FOLDER "Engine/Utils")