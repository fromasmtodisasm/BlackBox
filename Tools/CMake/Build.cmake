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
