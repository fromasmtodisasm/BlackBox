cmake_minimum_required(VERSION 3.15)
project(ScriptSystem CXX)
###################################################
add_library(${PROJECT_NAME})
###################################################
target_include_directories(
${PROJECT_NAME}
	PUBLIC	"${CMAKE_BINARY_DIR}"
	PUBLIC	"${CMAKE_SOURCE_DIR}/include/"
)

###################################################
find_package(LUA REQUIRED)
if (LUA_FOUND)
	target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
endif()
###################################################

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptObjectConsole.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptObjectRenderer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/FunctionHandler.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptBinding.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptObject.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptObjectScript.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptObjectSystem.cpp
  PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptSystem.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Math)

add_headers()
