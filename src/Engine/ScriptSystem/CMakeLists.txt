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
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LibScriptSystem.cpp
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/FunctionHandler.cpp
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptBinding.cpp
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptObject.cpp
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptObjectSystem.cpp
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/ScriptSystem.cpp

	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/AboutWnd.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/Coverage.cpp
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/Coverage.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/FileTree.cpp
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/FileTree.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/GoToFuncWnd.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/GoToWnd.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/LUADBG.cpp
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/LUADBG.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/LUADBG.ico
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinyBrowseFolder.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinyCaptionWindow.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinyCoolEdit.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinyFileEnum.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinyImageList.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinyMain.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinyRegistry.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinySplitter.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinyStatusBar.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinyTreeList.h
	PRIVATE ${SOURCE_DIR}/Engine/ScriptSystem/LuaDebugger/_TinyWindow.h
)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE Math)

add_private_headers()
target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "pch.hpp"
)
