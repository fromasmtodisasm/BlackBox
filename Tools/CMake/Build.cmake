include_guard(GLOBAL)

# Header guard to avoid duplicate execution when configuring (template) projects with OPTION_ENGINE
# Templates include Configure.cmake and add_subdirectory(${CRYENGINE_DIR}) which also includes Configure.cmake
# Templates do need Configure.cmake to set up some defines.
# In order to not have duplicate add_subdirectory calls (which causes errors), we need an include guard here.

# Sandbox Editor
if (OPTION_SANDBOX AND OPTION_STATIC_LINKING)
	message(STATUS "Disabling Sandbox - requires dynamic linking")
	set(OPTION_SANDBOX OFF)
endif()

if(OPTION_ENGINE OR OPTION_SANDBOX)
	# Add custom project with just listing of cmake files
	add_subdirectory("${TOOLS_CMAKE_DIR}")

	# Order currently matters for Engine, Games and Launchers
	# 1. CryEngine
	include ("${TOOLS_CMAKE_DIR}/BuildEngine.cmake")
endif()

# Only allow building legacy GameDLL's with the engine, assuming that we're not using the project system
if(OPTION_ENGINE AND NOT PROJECT_BUILD)
	# 2. Games
	add_subdirectories_glob("Code/Game*")
	add_subdirectory("${BLACKBOX_DIR}/Tools/")
endif()
	
if (OPTION_SANDBOX AND WINDOWS)
	if (NOT QT_SDK_ROOT)
		message(FATAL_ERROR "Qt Sdk Root Not Installed. --- ${QT_SDK_ROOT}")
	endif()
		# Find Qt before including any plugin subdirectories
	#if (MSVC_VERSION GREATER 1900) # Visual Studio > 2015
		set(QT_DIR "${QT_SDK_ROOT}/Qt/6.2.2/msvc2019_64")
		#message(STATUS "Qt Sdk Root: ${QT_SDK_ROOT}")
	#endif()
	set(Qt6_DIR "${QT_DIR}/lib/cmake/Qt6")
	set(QT_DEPLOY_ROOT "${QT_DIR}")
	list(APPEND CMAKE_PREFIX_PATH "${QT_DIR}/lib/cmake")


	#find_package(Qt6 COMPONENTS Core Gui OpenGL Widgets REQUIRED PATHS "${QT_DIR}" NO_DEFAULT_PATH)
	find_package(Qt6 COMPONENTS Core Gui OpenGL Widgets REQUIRED PATHS "${QT_DIR}")

	set(QT_DIR "${QT_DIR}" CACHE INTERNAL "QT directory" FORCE)
	set(Qt6_DIR "${Qt6_DIR}" CACHE INTERNAL "QT directory" FORCE)

	set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER  "${VS_FOLDER_PREFIX}/Sandbox/AUTOGEN")

	message(STATUS "Include Sandbox Editor")
	include ("${TOOLS_CMAKE_DIR}/BuildEditor.cmake")
endif()

if (OPTION_CONFIGURATOR)
  include ("${TOOLS_CMAKE_DIR}/BuildConfigurator.cmake")
endif()

# 5. Launchers
# we not under vcpkg package build
include ("${TOOLS_CMAKE_DIR}/BuildLaunchers.cmake")

if (OPTION_RC AND EXISTS "${BLACKBOX_DIR}/Code/Tools/RC")
if (0)
	include(ExternalProject)
	ExternalProject_Add(RC
		CMAKE_ARGS "-DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}" "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}" "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}" "-DCMAKE_BUILD_TYPE=$<$<CONFIG:Profile>:Release>$<$<NOT:$<CONFIG:Profile>>:$<CONFIG>>"
		SOURCE_DIR "${BLACKBOX_DIR}/Code/Tools/RC"
		BUILD_COMMAND "${CMAKE_COMMAND}" --build "." --config $<$<CONFIG:Profile>:Release>$<$<NOT:$<CONFIG:Profile>>:$<CONFIG>>
		INSTALL_COMMAND echo "Skipping install"
	)
else()
	add_subdirectory(Code/Tools/RC)
endif()
endif()

#message(STATUS "file for clangformat: ${ALL_PROJECT_SOURCES}")

#clang_format("${ALL_PROJECT_SOURCES}")

set(CMAKE_INSTALL_MESSAGE LAZY)
# FIXME
if (0)
	configure_file("${TOOLS_CMAKE_DIR}/modules/BlackBoxConfig.cmake.in" "${CMAKE_BINARY_DIR}/blackboxConfig.cmake")
	install(FILES "${CMAKE_BINARY_DIR}/blackboxConfig.cmake" DESTINATION share/blackbox)
endif()

install(DIRECTORY ${TOOLS_CMAKE_DIR} DESTINATION share/blackbox/Tools)

copy_binary_files_to_target()
