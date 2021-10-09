set(CMAKE_CXX_STANDARD 17)

# ##############################################################################
# Define the two required variables before including the source code for
# watching a git repository.

if(NEED_VC)
	set(PRE_CONFIGURE_FILE "VersionControl.cpp.in")
	set(POST_CONFIGURE_FILE ${CMAKE_BINARY_DIR}/VersionControl.cpp)
	include(${CMAKE_SOURCE_DIR}/Tools/CMake/GitWatcher.cmake)
endif()
# ##############################################################################

start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	#System
	System
	SOURCE_GROUP
	"Source Files"
	"CmdLine.cpp"
	"CmdLineArg.cpp"
	"CrySizerImpl.cpp"
	"Commands.cpp"
	"Console.cpp"
	"ConsoleBatchFile.cpp"
	"HardwareMouse.cpp"
	"Linux/Runtime.cpp"
	"NullLog.cpp"
	"MemoryManager.cpp"
	"RemoteConsole/RemoteConsole.cpp"
	"System.cpp"
	"SystemCFG.cpp"
	"SystemEventDispatcher.cpp"
	"SystemInit.cpp"
	"SystemRender.cpp"
	"SystemThreading.cpp"
	"Timer.cpp"
	"WindowsConsole.cpp"
	"XConsoleVariable.cpp"
	SOURCE_GROUP
	"File"
	"File/CryPak.cpp"
	SOURCE_GROUP
	"ScriptBinds"
	"ScriptBinds/ScriptObjectConsole.cpp"
	"ScriptBinds/ScriptObjectRenderer.cpp"
	"ScriptBinds/ScriptObjectScript.cpp"
	"ScriptBinds/ScriptObjectSound.cpp"
	SOURCE_GROUP
	"ProjectManager"
	"ProjectManager/ProjectManager.cpp"
	SOURCE_GROUP
	"Profiling"
	"Profiling/ProfilingSystem.cpp"
	"Profiling/ProfilingSystem.hpp"
	SOURCE_GROUP
	"XML"
	"XML/xml.h"
	"XML/xml.cpp"
	SOURCE_GROUP
	"Legacy"
	"Legacy/ISystem.h"
	"Legacy/System.cpp"
)
end_sources()

enginemodule(System FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER "Engine")

target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)
if(OPTION_USE_GUI)
	target_link_libraries(${THIS_PROJECT} PRIVATE GUI)
endif()

add_sdl(${THIS_PROJECT})
target_link_libraries(${THIS_PROJECT} PUBLIC tinyxml2::tinyxml2)

if(OPTION_USE_LINUXINPUT)
	target_compile_definitions(${THIS_PROJECT} PUBLIC -DUSE_LINUXINPUT)
endif()

if(NOT OPTION_STREAM_COOKIEFIED)
	target_compile_definitions(${THIS_PROJECT} PUBLIC -DNOT_COOKIEFIED)
endif()
if(OPTION_STREAM_ACTIVETYPECHECK)
	target_compile_definitions(${THIS_PROJECT} PUBLIC -DSTREAM_ACTIVETYPECHECK)
endif()
target_include_directories(${THIS_PROJECT}
													 PRIVATE ${BLACKBOX_ROOT}/include/System)
if(NEED_VC)
	set_target_properties(check_git_repository PROPERTIES FOLDER Tools)
endif()

target_include_directories(${THIS_PROJECT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
