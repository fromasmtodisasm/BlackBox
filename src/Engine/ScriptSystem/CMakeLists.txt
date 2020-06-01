cmake_minimum_required(VERSION 3.10)
project(ScriptSystem CXX)
###################################################
include(FindLua)
###################################################
add_library(${PROJECT_NAME})
###################################################
#[[
target_include_directories(
${PROJECT_NAME}
	PUBLIC	"${CMAKE_BINARY_DIR}"
	PUBLIC	"${CMAKE_SOURCE_DIR}/include/"
)
]]

###################################################
add_package(Lua)
target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE Utils)
###################################################

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/FunctionHandler.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptBinding.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptObject.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptObjectSystem.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptSystem.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Math)

add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)
