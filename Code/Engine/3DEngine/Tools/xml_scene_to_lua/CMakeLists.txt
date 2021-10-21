cmake_minimum_required(VERSION 3.10)

project(xml_scene_to_lua)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 3DEngine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine/Utils")
target_compile_definitions(${PROJECT_NAME} PRIVATE "USE_GLM")

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
