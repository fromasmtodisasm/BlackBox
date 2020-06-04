include_guard(GLOBAL)

# Header guard to avoid duplicate execution when configuring (template) projects with OPTION_ENGINE
# Templates include Configure.cmake and add_subdirectory(${CRYENGINE_DIR}) which also includes Configure.cmake
# Templates do need Configure.cmake to set up some defines.
# In order to not have duplicate add_subdirectory calls (which causes errors), we need an include guard here.

if(OPTION_ENGINE)
	# Add custom project with just listing of cmake files
	add_subdirectory("${TOOLS_CMAKE_DIR}")

	# Order currently matters for Engine, Games and Launchers
	# 1. CryEngine
	include ("${TOOLS_CMAKE_DIR}/BuildEngine.cmake")
endif()

# Only allow building legacy GameDLL's with the engine, assuming that we're not using the project system
if(OPTION_ENGINE AND NOT PROJECT_BUILD)
	# 2. Games
	#add_subdirectories_glob("Code/Game*")
endif()
	
if (OPTION_EDITOR)
	include ("${TOOLS_CMAKE_DIR}/BuildEditor.cmake")
endif()
if (OPTION_CONFIGURATOR)
  include ("${TOOLS_CMAKE_DIR}/BuildConfigurator.cmake")
endif()
# 5. Launchers
include ("${TOOLS_CMAKE_DIR}/BuildLaunchers.cmake")
add_subdirectory("${BLACKBOX_DIR}/src/Tools/")


message(STATUS "file for clangformat: ${ALL_PROJECT_SOURCES}")

clang_format("${ALL_PROJECT_SOURCES}")
