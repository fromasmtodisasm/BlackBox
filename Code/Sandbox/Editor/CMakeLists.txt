set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

start_sources()
# -- launcher.waf_files --
sources_platform(WINDOWS)
add_sources(
	"NoUberFile"
	SOURCE_GROUP "Header Files"
		"pch.hpp"
	SOURCE_GROUP "Core"
		"Include/SandboxAPI.h"
	SOURCE_GROUP "Resource Files"
		"resource.h"
	SOURCE_GROUP "Core"
		"EditorApp.cpp"
		"EditorApp.h"
		"EditorDefs.h"
		"EditorImpl.cpp"
		"EditorImpl.h"
		"Main.cpp"
		"MainWindow.cpp"
		"MainWindow.hpp"

		"MainMenu.cpp"
		"MainMenu.hpp"
		"GameEngine.cpp"
		"GameEngine.hpp"

		"PluginManager.cpp"
		"PluginManager.h"

		 "LogFile.h"

		 "SplashScreen.cpp"
		 "SplashScreen.h"

		 "ClassFactory.cpp"
		 "ClassFactory.h"
	SOURCE_GROUP "Util"
		 "Util/FileEnum.cpp"
		 "Util/FileEnum.h"
	SOURCE_GROUP "UI"
        "gamewindow.cpp"
        "gamewindow.h"
        "gamewindow.ui"
)

Editor(Sandbox PCH "pch.hpp" SOLUTION_FOLDER "Sandbox")
target_include_directories( ${THIS_PROJECT}
PUBLIC
	.
	Include
)
target_compile_definitions( ${THIS_PROJECT}
PRIVATE 
	-DDONT_BAN_STD_STRING
	-DFBXSDK_NEW_API=1
	-DNOT_USE_CRY_MEMORY_MANAGER
	-DSANDBOX_EDITOR_IMPL
)
