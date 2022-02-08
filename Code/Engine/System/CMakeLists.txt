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
	SOURCE_GROUP "HTTPDownloader"
		"DownloadManager.cpp"
		"HTTPDownloader.cpp"
		"DownloadManager.h"
		"HTTPDownloader.h"
	SOURCE_GROUP "Source Files"
		"CmdLine.cpp"
		"CmdLineArg.cpp"
		"Console.cpp"
		"ConsoleBatchFile.cpp"
		"CrySizerImpl.cpp"
		"DllMain.cpp"
		"HardwareMouse.cpp"
		"Linux/Runtime.cpp"
		"MemoryManager.cpp"
		"NullLog.cpp"
		"RemoteConsole/RemoteConsole.cpp"
		"System.cpp"
		"SystemCFG.cpp"
		"SystemEventDispatcher.cpp"
		"SystemInit.cpp"
		"SystemRender.cpp"
		"SystemThreading.cpp"
		"SystemWin32.cpp"
		"Timer.cpp"
		"WindowsConsole.cpp"
		"XConsoleVariable.cpp"
	SOURCE_GROUP "File"
		"File/CryPak.cpp"
		"PakVars.h"
	SOURCE_GROUP "ScriptBinds"
		"ScriptBinds/ScriptObjectConsole.cpp"
		"ScriptBinds/ScriptObjectRenderer.cpp"
		"ScriptBinds/ScriptObjectScript.cpp"
		"ScriptBinds/ScriptObjectSound.cpp"

		"ScriptBinds/ScriptObjectConsole.hpp"
		"ScriptBinds/ScriptObjectRenderer.hpp"
		"ScriptBinds/ScriptObjectScript.hpp"
		"ScriptBinds/ScriptObjectSound.hpp"
	SOURCE_GROUP "ProjectManager"
		"ProjectManager/ProjectManager.cpp"
	SOURCE_GROUP "Profiling"
		"Profiling/ProfilingSystem.cpp"
		"Profiling/ProfilingSystem.hpp"
	SOURCE_GROUP "XML"
		"XML/xml.h"
		"XML/xml.cpp"
	SOURCE_GROUP "Legacy"
		"Legacy/ISystem.h"
		"Legacy/System.cpp"
	SOURCE_GROUP "Font"
		"Font/NullFont.h"
		"Font/NullFont.cpp"
	SOURCE_GROUP "ScriptObject"
		"ScriptObjectSystem.cpp"
		"ScriptObjectSystem.hpp"
)
end_sources()

EngineModule(System PCH "pch.hpp" SOLUTION_FOLDER "Engine")

target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)

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
target_include_directories(${THIS_PROJECT} PRIVATE . ScriptBinds)
if(NEED_VC)
	set_target_properties(check_git_repository PROPERTIES FOLDER Tools)
endif()

target_include_directories(${THIS_PROJECT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_package(libzip)
add_package(libzippp)
target_link_libraries(${THIS_PROJECT} PUBLIC libzip::zip libzippp::libzippp)
if (WINDOWS)
	target_link_libraries(${THIS_PROJECT} PUBLIC Wininet)
endif()
