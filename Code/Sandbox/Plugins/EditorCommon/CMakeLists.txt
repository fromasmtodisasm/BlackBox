set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

start_sources()
# -- launcher.waf_files --
sources_platform(WINDOWS)
add_sources(
	"NoUberFile"
	SOURCE_GROUP "Header Files"
		"pch.hpp"
	SOURCE_GROUP "Resource Files"
		"Resource.h"
	SOURCE_GROUP "Source Files"
		"EditorCommon.h"
		"EditorCommonAPI.h"
		"EditorCommonInit.h"
		"EditorCommonInit.cpp"
		"ImGuiManager.hpp"
		"IEditorClassFactory.h"
		"ViewPane.h"

		"AutoRegister.cpp"
		"AutoRegister.h"

		"IPlugin.h"
		"Lib.cpp"
)

EngineModule(EditorCommon EDITOR_COMPILE_SETTINGS PCH "pch.hpp" SOLUTION_FOLDER "Sandbox")

target_compile_definitions(${THIS_PROJECT} PRIVATE
	-DEDITOR_COMMON_EXPORTS 
	-DNOT_USE_CRY_MEMORY_MANAGER
	-DDONT_USE_CRY_MEMORY_MANAGER
)

#TODO: remove dependencies from editor common to editor!
target_include_directories(${THIS_PROJECT} PRIVATE "${BLACKBOX_DIR}/Code/Sandbox/Editor")	
target_include_directories(${THIS_PROJECT} PRIVATE "${BLACKBOX_DIR}/Code/Sandbox/Editor/Include")

target_link_libraries(${THIS_PROJECT} PRIVATE GUI)