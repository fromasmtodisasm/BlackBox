start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	ScriptSystem
	SOURCE_GROUP
	"Source Files"
	"LibScriptSystem.cpp"
	"FunctionHandler.cpp"
	"ScriptBinding.cpp"
	"ScriptObject.cpp"
	"ScriptSystem.cpp"
	#[[
		"LuaDebugger/AboutWnd.h"
		"LuaDebugger/Coverage.cpp"
		"LuaDebugger/Coverage.h"
		"LuaDebugger/FileTree.cpp"
		"LuaDebugger/FileTree.h"
		"LuaDebugger/GoToFuncWnd.h"
		"LuaDebugger/GoToWnd.h"
		"LuaDebugger/LUADBG.cpp"
		"LuaDebugger/LUADBG.h"
		"LuaDebugger/LUADBG.ico"
		"LuaDebugger/_TinyBrowseFolder.h"
		"LuaDebugger/_TinyCaptionWindow.h"
		"LuaDebugger/_TinyCoolEdit.h"
		"LuaDebugger/_TinyFileEnum.h"
		"LuaDebugger/_TinyImageList.h"
		"LuaDebugger/_TinyMain.h"
		"LuaDebugger/_TinyRegistry.h"
		"LuaDebugger/_TinySplitter.h"
		"LuaDebugger/_TinyStatusBar.h"
		"LuaDebugger/_TinyTreeList.h"
		"LuaDebugger/_TinyWindow.h"
		]]
)
end_sources()

EngineModule(ScriptSystem PCH "pch.hpp" SOLUTION_FOLDER
						 "Engine")
# ##############################################################################
include(FindLua)
# ##############################################################################
add_package(Lua)
target_include_directories(${THIS_PROJECT} PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(${THIS_PROJECT} PRIVATE ${LUA_LIBRARIES})

target_link_libraries(${THIS_PROJECT} PRIVATE Utils)
# ##############################################################################

target_include_directories(${THIS_PROJECT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${THIS_PROJECT} PRIVATE Math)
