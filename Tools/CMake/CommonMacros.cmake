if(NOT WIN32)
	string(ASCII 27 Esc)
	set(ColourReset "${Esc}[m")
	set(ColourBold "${Esc}[1m")
	set(Red "${Esc}[31m")
	set(Green "${Esc}[32m")
	set(Yellow "${Esc}[33m")
	set(Blue "${Esc}[34m")
	set(Magenta "${Esc}[35m")
	set(Cyan "${Esc}[36m")
	set(White "${Esc}[37m")
	set(BoldRed "${Esc}[1;31m")
	set(BoldGreen "${Esc}[1;32m")
	set(BoldYellow "${Esc}[1;33m")
	set(BoldBlue "${Esc}[1;34m")
	set(BoldMagenta "${Esc}[1;35m")
	set(BoldCyan "${Esc}[1;36m")
	set(BoldWhite "${Esc}[1;37m")
endif()

function(message)
	list(GET ARGV 0 MessageType)
	if(MessageType STREQUAL FATAL_ERROR OR MessageType STREQUAL SEND_ERROR)
		list(REMOVE_AT ARGV 0)
		_message(${MessageType} "${BoldRed}${ARGV}${ColourReset}")
	elseif(MessageType STREQUAL WARNING)
		list(REMOVE_AT ARGV 0)
		_message(${MessageType} "${BoldYellow}${ARGV}${ColourReset}")
	elseif(MessageType STREQUAL AUTHOR_WARNING)
		list(REMOVE_AT ARGV 0)
		_message(${MessageType} "${BoldCyan}${ARGV}${ColourReset}")
	elseif(MessageType STREQUAL STATUS)
		list(REMOVE_AT ARGV 0)
		_message(${MessageType} "${Green}${ARGV}${ColourReset}")
	else()
		_message("${ARGV}")
	endif()
endfunction()

function(add_private_headers)
	file(GLOB_RECURSE INCLUDE_FILES
			 ${BLACKBOX_PRIVATE_INCLUDE_DIRS}/${PROJECT_NAME}/*.hpp
			 ${BLACKBOX_PRIVATE_INCLUDE_DIRS}/${PROJECT_NAME}/*.h)
	file(GLOB_RECURSE INCLUDE_FILES1 ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
			 ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
	target_sources(${PROJECT_NAME} PRIVATE ${INCLUDE_FILES} ${INCLUDE_FILES1})
	source_group(
		TREE ${BLACKBOX_PRIVATE_INCLUDE_DIRS}/${PROJECT_NAME}
		PREFIX "Header Files"
		FILES ${INCLUDE_FILES})
	source_group(
		TREE ${CMAKE_CURRENT_SOURCE_DIR}
		PREFIX "Header Files"
		FILES ${INCLUDE_FILES1})
endfunction()

function(install_package Package Result)

	# set(ToInstall "${Package}:${CMAKE_GENERATOR_PLATFORM}-${BB_PLATFORM}")
	set(ToInstall "${Package}:${VCPKG_TARGET_TRIPLET}")
	set(CMD "vcpkg")
	set(ARGS " install ${ToInstall}")
	message(STATUS "Trying install package [${ToInstall}]")
	message(${VCPKG})
	execute_process(COMMAND ${VCPKG} install "${ToInstall}"
									RESULT_VARIABLE result)
	set(${Result}
			${result}
			PARENT_SCOPE)
endfunction()

macro(FindGit)
	find_package(Git)
	if(NOT GIT_FOUND)
		message(FATAL_ERROR "Git not found")
	else()
		message(STATUS "Git FOUND at ${GIT_EXECUTABLE}")
	endif()
endmacro()

macro(InstallVcpkg)
	find_program(
		VCPKG
		NAMES vcpkg
		PATHS ${CMAKE_SOURCE_DIR}/vcpkg)
	if(NOT VCPKG)
		include(${TOOLS_CMAKE_DIR}/PrepareVcpkg.cmake)
		# PrepareVcpkg()
		find_program(VCPKG NAMES vcpkg)
		if(NOT VCPKG)
			message(FATAL_ERROR "vcpkg not found, var: ${VCPKG}")
		endif()
	else()
		message(STATUS "VCPKG FOUND at [${VCPKG}]")
	endif()
endmacro()

macro(CommonMacrosInit)
	message(STATUS "INITIALIZE COMMON MACROS")
	if((DEFINED LINUX OR MINGW) AND (NOT DEFINED VCPKG_INSTALLER))
		include(FindPkgConfig)
	elseif(DEFINED LINUX AND (DEFINED VCPKG_INSTALLER))
		set(BB_PLATFORM linux)
	else()
		set(BB_PLATFORM windows)
	endif()
	# InstallVcpkg()
	if (ADD_PACKAGE_DEP_MODE)
		file(REMOVE ${CMAKE_BINARY_DIR}/.deps)
	endif()
endmacro()

function(dump_vars)
	message(STATUS ==============)
	message(STATUS "Dumping ${ARGC} variables:")
	foreach(VAR_NAME ${ARGN})
		message(STATUS " ${VAR_NAME} == \"${${VAR_NAME}}\"")
	endforeach()
	message(STATUS ==============)
endfunction()

function(add_package_unix_or_cygwin Package)
	find_package(${ARGV})
endfunction()

function(add_package_msvc Package)
	if(NOT DEFINED VCPKG_INSTALLER)
		find_package(${Package} REQUIRED)
	else(NOT ${Package}_FOUND)
		if (0)
			set(result OFF)
			if(NOT ${Package}_INSTALLED)
				message(STATUS "${Package} yet not installed")
				install_package("${Package}" result)
				if(NOT ${result} EQUAL 0)
					message(FATAL_ERROR "could not install ${Package}")
				else()
					set(${Package}_INSTALLED
							ON
							CACHE STRING "Installed package" FORCE)
				endif()
			endif()
		endif()
		message(STATUS "FIND CONFIG PACKAGE: ${Package}")
		find_package(${Package} CONFIG)
		if(NOT ${Package}_FOUND)
			message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
			find_package(${Package} REQUIRED)
		endif()
	endif()
endfunction()

function(add_dep Package)
	file(APPEND ${CMAKE_BINARY_DIR}/.deps "${Package}\n")
endfunction()

function(add_package Package)
	if (ADD_PACKAGE_DEP_MODE)
		add_dep(${Package})
	endif()
	find_package(${Package})
	#find_package(${ARGV})
	add_package_msvc(${Package})
endfunction()

# Organize projects into solution folders
macro(set_solution_folder folder target)
	if(TARGET ${target})
		if(NOT "${folder}" MATCHES "^Projects" AND NOT "${folder}" MATCHES
																							 "^Project")
			set_property(TARGET ${target} PROPERTY FOLDER
																						 "${VS_FOLDER_PREFIX}/${folder}")
		else()
			set_property(TARGET ${target} PROPERTY FOLDER "${folder}")
		endif()
	endif()
endmacro()

# Helper macro to set default StartUp Project in Visual Studio
macro(set_solution_startup_target target)
	if("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
		# Set startup project to launch Game.exe with this project
		set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
								 PROPERTY VS_STARTUP_PROJECT ${target})
	endif()
endmacro()

macro(SET_PLATFORM_TARGET_PROPERTIES THIS_PROJECT)
	target_compile_definitions(
		${THIS_PROJECT} PRIVATE "-DCODE_BASE_FOLDER=\"${CRYENGINE_DIR}/Code/\"")
	target_link_libraries(${THIS_PROJECT} PRIVATE ${COMMON_LIBS})

	if(OPTION_DEVELOPER_CONSOLE_IN_RELEASE)
		target_compile_definitions(${THIS_PROJECT}
															 PRIVATE "-DENABLE_DEVELOPER_CONSOLE_IN_RELEASE")
	endif()

	if(OPTION_REMOTE_CONSOLE)
		target_compile_definitions(${THIS_PROJECT} PRIVATE "-DUSE_REMOTE_CONSOLE")
	endif()
endmacro()
function(add_subsystem subsystem)
	add_subdirectory(${ENGINE_DIR}/${subsystem} ${subsystem})
	# target_link_libraries(${BLACKBOX_PROJECT} INTERFACE ${subsystem})
	# set_target_properties(${subsystem} PROPERTIES FOLDER "Engine")

	# get_target_property(SOURCE_FILES ${subsystem} SOURCES)
	# apply_compile_settings(${subsystem})

	list(APPEND ALL_PROJECT_SOURCES ${SOURCE_FILES})
	list(APPEND ALL_PROJECT_SYSTEMS ${subsystem})
	# install_this(${subsystem})
endfunction()

macro(export_subsystem subsystem)

endmacro()

macro(install_this THIS_PROJECT)
	# ############################################################################
	install(
		TARGETS ${THIS_PROJECT}
		EXPORT ${THIS_PROJECT}Config
		# RUNTIME DESTINATION bin
		# LIBRARY DESTINATION lib
		# ARCHIVE DESTINATION lib/static
	)

	# This makes the project importable from the install directory Put config file
	# in per-project dir (name MUST match), can also just go into 'cmake'.
	# install(EXPORT ${THIS_PROJECT}Config DESTINATION .)

	# This makes the project importable from the build directory
	export(TARGETS ${THIS_PROJECT} FILE ${THIS_PROJECT}Config.cmake)
endmacro()

#[[
	function(target_precompile_headers _target)
	if (${CMAKE_VERSION} VERSION_LESS "3.16.0")
		cmake_parse_arguments(
		  _ARGS
		  ""
		  "NOTUSED"
		  "INTERFACE;PRIVATE;PUBLIC"
		  ${ARGN}
		  )
		message("precomiled headers for [${_target}]")
		foreach(interface ${_ARGS_INTERFACE})
		  message(STATUS "interface pch ${interface}")
		endforeach()
		foreach(public ${_ARGS_PUBLIC})
		  message(STATUS "public pch ${public}")
		endforeach()
		foreach(private ${_ARGS_PRIVATE})
		  message(STATUS "private pch ${private}")
		endforeach()
	else()
		_target_precompile_headers(${ARGV})
	endif()

	endfunction()
	]]

function(add_sdl target)
	if(DEFINED LINUX OR DEFINED MINGW)
		if(NOT DEFINED VCPKG_INSTALLER)
			pkg_check_modules(SDL2 SDL2)
			target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
			target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
			message(STATUS "SDL2 include ${SDL2_INCLUDE_DIRS}")

			#[[
			find_package(SDL2 CONFIG REQUIRED)
			target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)

			target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY})
			target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})
			]]

		else()
			add_package(sdl2)
			target_link_libraries(${target} PRIVATE SDL2::SDL2)
		endif()
	else()
		add_package(sdl2)
		target_link_libraries(${target} PRIVATE SDL2::SDL2)
	endif()
endfunction()

macro(read_settings)
	set(options DISABLE_MFC FORCE_STATIC FORCE_SHARED FORCE_SHARED_WIN
							EDITOR_COMPILE_SETTINGS MSVC_PERMISSIVE)
	set(oneValueArgs SOLUTION_FOLDER PCH OUTDIR)
	set(multiValueArgs FILE_LIST INCLUDES LIBS DEFINES)
	cmake_parse_arguments(MODULE "${options}" "${oneValueArgs}"
												"${multiValueArgs}" ${ARGN})
	if(MODULE_PCH)
		# string(REPLACE ".cpp" ".h" MODULE_PCH_HEADER_FILE ${MODULE_PCH})
		# get_filename_component(MODULE_PCH_H ${MODULE_PCH_HEADER_FILE} NAME)
		# target_precompile_headers() target_precompile_headers(${THIS_PROJECT}
		# PRIVATE ${MODULE_PCH} )
	endif()
	if(MODULE_FORCE_SHARED_WIN AND WINDOWS)
		set(MODULE_FORCE_SHARED TRUE)
	endif()
	if(MODULE_FORCE_SHARED AND (ORBIS))
		set(MODULE_FORCE_SHARED FALSE)
	endif()
endmacro()
macro(prepare_project)
	set(THIS_PROJECT
			${target}
			PARENT_SCOPE)
	set(THIS_PROJECT ${target})
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
	project(${target})
	read_settings(${ARGN})
	# generate_uber_files()
	if(NOT ${THIS_PROJECT}_SOURCES)
		set(${THIS_PROJECT}_SOURCES ${SOURCES})
	else()

	endif()
endmacro()
# Process source files
macro(start_sources)
	set(SOURCES)
	set(SOURCE_GROUPS)
	set(UBERFILES)
endmacro()

# Specifies a set of platforms that should build source files provided after
# this point. Specify ALL to build for all platforms. Syntax:
# sources_platform([OR] X Y Z [AND A B C]) Subsequent source files will be built
# if "if(T)" is true for at least one token T in OR and all tokens T in AND.
# Example: sources_platform(WINDOWS ANDROID AND HAS_FOO) = build if target is
# Windows or Android, and HAS_FOO is true.
macro(sources_platform)
	set(PLATFORM_CONDITION)
	set(multiValueArgs OR AND)
	cmake_parse_arguments(COND "" "" "${multiValueArgs}" ${ARGN})
	list(APPEND COND_OR ${COND_UNPARSED_ARGUMENTS})
	set(PLATFORM_CONDITION FALSE)
	foreach(c ${COND_OR})
		if(${c} STREQUAL "ALL")
			set(c TRUE)
		endif()
		if(${c})
			set(PLATFORM_CONDITION TRUE)
		endif()
	endforeach()
	foreach(c ${COND_AND})
		if(${c} STREQUAL "ALL")
			set(c TRUE)
		endif()
		if(NOT ${c})
			set(PLATFORM_CONDITION FALSE)
		endif()
	endforeach()
endmacro()

macro(add_files)
	foreach(p ${UB_PROJECTS})
		list(APPEND ${p}_SOURCES ${ARGN})
	endforeach()
	list(APPEND SOURCES ${ARGN})
endmacro()

# Usage: add_sources(uberFileName [PROJECTS proj1 proj2 ...] (SOURCE_GROUP
# "GroupName" file1 file2...)+ Use "NoUberFile" to signify source files which
# should not be compiled with others. PROJECTS is optional, but should be used
# when building multiple projects with distinct sets of source files from one
# directory. If a project is never referenced in PROJECTS, it will use all
# sources given in this directory. Added files will be built if building for a
# platform matching the latest sources_platform call. If the files should not be
# built, they are allowed to be missing on the filesystem.
macro(add_sources name)
	set(multiValueArgs PROJECTS SOURCE_GROUP)
	cmake_parse_arguments(UB "" "" "${multiValueArgs}" ${ARGN})

	if(NOT "${name}" STREQUAL "NoUberFile")
		list(APPEND UBERFILES ${name})
		set(${name}_PROJECTS ${UB_PROJECTS})
	endif()

	# Parse source groups manually to avoid conflating multiple groups
	set(CURRENT_SOURCE_GROUP)
	set(EXPECTING_FILE FALSE)
	set(EXPECTING_GROUP_NAME FALSE)
	foreach(ARG ${ARGN})
		if(${ARG} STREQUAL "PROJECTS")
			set(EXPECTING_FILE FALSE)
		elseif(${ARG} STREQUAL "SOURCE_GROUP")
			set(EXPECTING_GROUP_NAME TRUE)
		elseif(EXPECTING_GROUP_NAME)
			set(CURRENT_SOURCE_GROUP ${ARG})
			string(REPLACE " " "_" CURRENT_SOURCE_GROUP_VAR ${CURRENT_SOURCE_GROUP})
			list(FIND SOURCE_GROUPS ${CURRENT_SOURCE_GROUP_VAR} GROUP_INDEX)
			if(GROUP_INDEX EQUAL -1)
				list(APPEND SOURCE_GROUPS ${CURRENT_SOURCE_GROUP_VAR})
				set(SOURCE_GROUP_${CURRENT_SOURCE_GROUP_VAR})
			endif()
			set(EXPECTING_GROUP_NAME FALSE)
			set(EXPECTING_FILE TRUE)
		elseif(EXPECTING_FILE)
			if(NOT CURRENT_SOURCE_GROUP)
				message(FATAL_ERROR "No source group name defined")
			endif()
			list(APPEND SOURCE_GROUP_${CURRENT_SOURCE_GROUP_VAR} ${ARG})
			if(NOT ${CURRENT_SOURCE_GROUP} STREQUAL "Root")
				source_group(${CURRENT_SOURCE_GROUP} FILES ${ARG})
			else()
				source_group("" FILES ${ARG})
			endif()
			# .mm files are Objective-C; disable those from build on non-Apple if(NOT
			# (${PLATFORM_CONDITION}) OR (NOT APPLE AND ${ARG} MATCHES ".*\\.\\mm$"))
			# if (EXISTS ${ARG})
			if(TRUE)
				# set_source_files_properties(${ARG} PROPERTIES HEADER_FILE_ONLY TRUE)
				add_files(${ARG})
			else()
				message(STATUS "this file not exist!!!: ${ARG}")
			endif()
			# else()
			# message(STATUS "adding UBERFILE: ${ARG}") add_to_uberfile(${name}
			# ${ARG})
			# endif()
		endif()
	endforeach()
endmacro()

macro(end_sources)

endmacro()

# Automatically add sources in current CMakeLists directory, respecting
# directory structure
macro(add_sources_recursive_search)
	set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")
	file(
		GLOB_RECURSE _source_list
		LIST_DIRECTORIES false
		"${_src_root_path}/*.cpp" "${_src_root_path}/*.h" "${_src_root_path}/*.hpp")

	foreach(_source IN ITEMS ${_source_list})
		get_filename_component(_source_path "${_source}" PATH)
		file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
		string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
		source_group("${_group_path}" FILES "${_source}")
		file(RELATIVE_PATH _source_rel "${_src_root_path}" "${_source}")

		if(_group_path STREQUAL "")
			set(_group_path "Root")
		endif()

		string(REPLACE "\\" "_" _group_path "${_group_path}")

		set(_group_path "${_group_path}.cpp")

		list(FIND UBERFILES ${_group_path} GROUP_INDEX)
		if(GROUP_INDEX EQUAL -1)
			list(APPEND UBERFILES "${_group_path}")
			set(${_group_path}_PROJECTS ${UB_PROJECTS})
		endif()

		# add_to_uberfile(${_group_path} ${_source_rel})
	endforeach()
endmacro()

macro(remove_files_from_list filelist filepattern)
	# message(STATUS " remove_files_from_list ${filelist} ${filepattern}")
	set(FilesToRemove "")
	foreach(sourcefile ${${filelist}})
		if("${sourcefile}" MATCHES ${filepattern})
			list(APPEND FilesToRemove "${sourcefile}")
			# message(STATUS " FILE TO REMOVE: ${sourcefile}")
		endif()
	endforeach()
	foreach(sourcefile ${FilesToRemove})
		list(REMOVE_ITEM ${filelist} "${sourcefile}")
	endforeach()
endmacro()

macro(apply_compile_settings)
	if(MODULE_PCH)
		# CryQt defines incompatible DLLExport in stdafx, temporarily disable PCH
		# for CryQt now
		if(NOT "${THIS_PROJECT}" STREQUAL "CryQt")
			# USE_MSVC_PRECOMPILED_HEADER( ${THIS_PROJECT} ${MODULE_PCH_H}
			# ${MODULE_PCH} )
			message(STATUS "target precompiled header = ${MODULE_PCH}")
			target_precompile_headers(${THIS_PROJECT} PRIVATE ${MODULE_PCH})
		endif()
		set_property(
			TARGET ${THIS_PROJECT}
			APPEND
			PROPERTY AUTOMOC_MOC_OPTIONS -b${MODULE_PCH_H})
	endif()
	if(MODULE_OUTDIR)
		set_property(
			TARGET ${THIS_PROJECT}
			PROPERTY LIBRARY_OUTPUT_DIRECTORY
							 "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${MODULE_OUTDIR}")
		set_property(
			TARGET ${THIS_PROJECT}
			PROPERTY RUNTIME_OUTPUT_DIRECTORY
							 "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${MODULE_OUTDIR}")
	endif()
	set_platform_target_properties(${THIS_PROJECT})
	if(MODULE_SOLUTION_FOLDER)
		set_solution_folder("${MODULE_SOLUTION_FOLDER}" ${THIS_PROJECT})
	endif()

	if(WINDOWS AND NOT OPTION_DEDICATED_SERVER)
		target_compile_options(${THIS_PROJECT}
													 PRIVATE $<$<CONFIG:Release>:-DPURE_CLIENT>)
	endif()

	if(DEFINED PROJECT_BUILD_CRYENGINE AND NOT PROJECT_BUILD_CRYENGINE)
		# If option to not build engine modules is selected they are excluded from
		# the build
		set_target_properties(${THIS_PROJECT} PROPERTIES EXCLUDE_FROM_ALL TRUE)
		set_target_properties(${THIS_PROJECT} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD
																										 TRUE)
	endif()

	get_target_property(target_type ${THIS_PROJECT} TYPE)
	if(target_type MATCHES "EXECUTABLE")
		target_compile_options(${THIS_PROJECT} PRIVATE -DCRY_IS_APPLICATION)
	endif()
endmacro()
function(EngineModule target)
	prepare_project(${ARGN})

	if((OPTION_STATIC_LINKING OR MODULE_FORCE_STATIC) AND NOT MODULE_FORCE_SHARED)
		# Static modules must not contain resource files
		remove_files_from_list(${THIS_PROJECT}_SOURCES ".*\\.rc$")
	endif()

	if(MODULE_FORCE_SHARED)
		add_library(${THIS_PROJECT} SHARED ${${THIS_PROJECT}_SOURCES})
	elseif(MODULE_FORCE_STATIC)
		add_library(${THIS_PROJECT} STATIC ${${THIS_PROJECT}_SOURCES})
	else()
		add_library(${THIS_PROJECT} ${${THIS_PROJECT}_SOURCES})
	endif()
	if(MODULE_PCH)
		# string(REPLACE ".cpp" ".h" MODULE_PCH_HEADER_FILE ${MODULE_PCH})
		# get_filename_component(MODULE_PCH_H ${MODULE_PCH_HEADER_FILE} NAME)
		# target_precompile_headers()
		target_precompile_headers(${THIS_PROJECT} PRIVATE ${MODULE_PCH})
	endif()

	# message(STATUS "Adding sources for target ${THIS_PROJECT}")
	foreach(src include ${${THIS_PROJECT}_SOURCES})
		# message(STATUS "!!!: ${src}")
	endforeach()

	if(MODULE_EDITOR_COMPILE_SETTINGS)
		set_editor_module_flags()
	elseif(NOT MODULE_FORCE_SHARED)
		set(MODULES
				${MODULES} ${THIS_PROJECT}
				CACHE INTERNAL "List of engine being built" FORCE)
	endif()

	if(MSVC)
		if(${THIS_PROJECT} STREQUAL "EditorCommon" OR ${THIS_PROJECT} STREQUAL
																									"MFCToolsPlugin")
			# These editor plugins are built using EngineModule, but need to have RTTI
			# enabled.
			target_compile_options(${THIS_PROJECT} PRIVATE /GR)
		else()
			target_compile_options(${THIS_PROJECT} PRIVATE /GR-)
		endif()
	endif()

	apply_compile_settings()
	if((OPTION_STATIC_LINKING OR MODULE_FORCE_STATIC) AND NOT MODULE_FORCE_SHARED)
		target_compile_definitions(${THIS_PROJECT} PRIVATE _LIB
																											 -DIS_MONOLITHIC_BUILD)
	endif()
	add_metadata()
	if(ANDROID
		 AND NOT OPTION_STATIC_LINKING
		 AND NOT MODULE_FORCE_STATIC)
		set(SHARED_MODULES
				${SHARED_MODULES} ${THIS_PROJECT}
				CACHE INTERNAL "Shared modules for APK creation" FORCE)
		target_link_libraries(${THIS_PROJECT} PRIVATE m log c android)
	endif()

	if(OPTION_DEDICATED_SERVER)
		target_compile_definitions(${THIS_PROJECT} PRIVATE "-DDEDICATED_SERVER")
	endif()

	if(NOT DEFINED PROJECT_BUILD_CRYENGINE OR PROJECT_BUILD_CRYENGINE)
		install(
			TARGETS ${target}
			LIBRARY DESTINATION bin
			RUNTIME DESTINATION bin
			ARCHIVE DESTINATION lib)
	endif()

	# apply_ltcg_if_enabled()

endfunction()

function(GameModule target)
	prepare_project(${ARGN})
	if(OPTION_STATIC_LINKING AND NOT OPTION_STATIC_LINKING_WITH_GAME_AS_DLL)
		add_library(${THIS_PROJECT} ${${THIS_PROJECT}_SOURCES})
	else()
		add_library(${THIS_PROJECT} SHARED ${${THIS_PROJECT}_SOURCES})
	endif()
	apply_compile_settings()
	set(game_folder
			${CMAKE_CURRENT_SOURCE_DIR}
			CACHE INTERNAL "Game folder used for resource files on Windows" FORCE)
	set(GAME_MODULES
			${GAME_MODULES} ${THIS_PROJECT}
			CACHE INTERNAL "List of game modules being built" FORCE)
	list(LENGTH GAME_MODULES NUM_GAME_MODULES)
	if(NUM_GAME_MODULES GREATER 1 AND NOT OPTION_RUNTIME_CVAR_OVERRIDES)
		message(
			FATAL_ERROR
				" OPTION_RUNTIME_CVAR_OVERRIDES should be enabled when having more than one project enabled. Add sys_cvar_overrides_path = <path to CVarOverrides.h> e.g. Code/GameSDK/GameDLL/CVarOverrides.h to your system.cfg"
		)
	endif()

	if(OPTION_STATIC_LINKING AND NOT OPTION_STATIC_LINKING_WITH_GAME_AS_DLL)
		target_compile_definitions(${THIS_PROJECT}
															 PRIVATE _LIB -DCRY_IS_MONOLITHIC_BUILD)
	elseif(ANDROID)
		set(SHARED_MODULES
				${SHARED_MODULES} ${THIS_PROJECT}
				CACHE INTERNAL "Shared modules for APK creation" FORCE)
		target_link_libraries(${THIS_PROJECT} PRIVATE m log c android)
	endif()

	add_metadata()

	if(OPTION_DEDICATED_SERVER)
		target_compile_definitions(${THIS_PROJECT} PRIVATE "-DDEDICATED_SERVER")
	endif()

	if(NOT DEFINED PROJECT_BUILD_CRYENGINE OR PROJECT_BUILD_CRYENGINE)
		install(
			TARGETS ${target}
			LIBRARY DESTINATION bin
			RUNTIME DESTINATION bin
			ARCHIVE DESTINATION lib)
	endif()

	file(READ "${CMAKE_BINARY_DIR}/ProjectCVarOverrides.h"
			 project_cvar_overrides_h_content)
	if(EXISTS "${game_folder}/CVarOverrides.h" AND NOT
																								 OPTION_RUNTIME_CVAR_OVERRIDES)
		if(NOT project_cvar_overrides_h_content STREQUAL
			 "#include \"${game_folder}/CVarOverrides.h\"")
			file(WRITE "${CMAKE_BINARY_DIR}/ProjectCVarOverrides.h"
					 "#include \"${game_folder}/CVarOverrides.h\"")
		endif()
	else()
		if(NOT project_cvar_overrides_h_content STREQUAL "")
			file(WRITE "${CMAKE_BINARY_DIR}/ProjectCVarOverrides.h" "")
		endif()
	endif()

	file(READ "${CMAKE_BINARY_DIR}/ProjectCVarWhitelist.h"
			 project_cvar_whitelist_h_content)
	if(EXISTS "${game_folder}/CVarWhitelist.h" AND NOT
																								 OPTION_RUNTIME_CVAR_OVERRIDES)
		if(NOT project_cvar_whitelist_h_content STREQUAL
			 "#include \"${game_folder}/CVarWhitelist.h\"")
			file(WRITE "${CMAKE_BINARY_DIR}/ProjectCVarWhitelist.h"
					 "#include \"${game_folder}/CVarWhitelist.h\"")
		endif()
	else()
		if(NOT project_cvar_overrides_h_content STREQUAL "")
			file(WRITE "${CMAKE_BINARY_DIR}/ProjectCVarWhitelist.h" "")
		endif()
	endif()

	file(READ "${CMAKE_BINARY_DIR}/ProjectEngineDefineOverrides.h"
			 project_engine_define_overrides_h_content)
	if(EXISTS "${game_folder}/EngineDefineOverrides.h"
		 AND NOT OPTION_RUNTIME_CVAR_OVERRIDES)
		if(NOT project_engine_define_overrides_h_content STREQUAL
			 "#include \"${game_folder}/EngineDefineOverrides.h\"")
			file(WRITE "${CMAKE_BINARY_DIR}/ProjectEngineDefineOverrides.h"
					 "#include \"${game_folder}/EngineDefineOverrides.h\"")
		endif()
	else()
		if(NOT project_engine_define_overrides_h_content STREQUAL "")
			file(WRITE "${CMAKE_BINARY_DIR}/ProjectEngineDefineOverrides.h" "")
		endif()
	endif()

	if(EXISTS "${game_folder}/EngineDefineOverrides.h"
		 AND OPTION_RUNTIME_CVAR_OVERRIDES)
		message(
			WARNING
				"Disabling project engine define overrides because OPTION_RUNTIME_CVAR_OVERRIDES was enabled!"
		)
	endif()

	# apply_ltcg_if_enabled()

endfunction()

function(add_SDL_net)
	if(DEFINED LINUX OR DEFINED MINGW)
		if(NOT DEFINED VCPKG_INSTALLER)
			pkg_check_modules(SDL2_NET SDL2_net)
			target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_NET_LIBRARIES})
			target_include_directories(${PROJECT_NAME}
																 PRIVATE ${SDL2_NET_INCLUDE_DIRS})
			message(STATUS "SDL2 net include ${SDL2_NET_INCLUDE_DIRS}")
		else()
			add_package(sdl2-net)
			target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_net)
		endif()
	else()
		add_package(sdl2-net)
		target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_net)
	endif()
endfunction()

function(Launcher target)
	message(STATUS "Launcher target: ${target}")
	prepare_project(${ARGN})
	if(ANDROID)
		add_library(${target} SHARED ${${THIS_PROJECT}_SOURCES})
		target_link_libraries(${THIS_PROJECT} PRIVATE m log c android)
		configure_android_launcher(${target})
	elseif(WINDOWS)
		add_executable(${THIS_PROJECT} WIN32 ${${THIS_PROJECT}_SOURCES})
	else()
		add_executable(${target} ${${THIS_PROJECT}_SOURCES})
	endif()
	if(ORBIS)
		set_property(TARGET ${target} PROPERTY OUTPUT_NAME
																					 "${OPTION_LAUNCHER_EXECUTABLE}.elf")
	elseif(NOT ANDROID)
		set_property(TARGET ${THIS_PROJECT}
								 PROPERTY OUTPUT_NAME "${OPTION_LAUNCHER_EXECUTABLE}")
	endif()
	if(DURANGO)
		# Set empty so the output directory will default to the solution directory
		set_property(TARGET ${target} PROPERTY ARCHIVE_OUTPUT_DIRECTORY "")
	endif()
	if(OPTION_STATIC_LINKING)
		use_scaleform()
		target_compile_definitions(${THIS_PROJECT}
															 PRIVATE _LIB -DCRY_IS_MONOLITHIC_BUILD)
		if(WINDOWS)
			set_property(
				TARGET ${THIS_PROJECT}
				APPEND_STRING
				PROPERTY LINK_FLAGS_PROFILE " /NODEFAULTLIB:libcpmt.lib")
			set_property(
				TARGET ${THIS_PROJECT}
				APPEND_STRING
				PROPERTY LINK_FLAGS_RELEASE " /NODEFAULTLIB:libcpmt.lib")
		endif()
		if(OPTION_STATIC_LINKING_WITH_GAME_AS_DLL)
			set(MODULES_LIST ${MODULES})
		else()
			set(MODULES_LIST ${GAME_MODULES} ${MODULES})
		endif()
		wrap_whole_archive(${target} WRAPPED_MODULES MODULES_LIST)
		target_link_libraries(${THIS_PROJECT} PRIVATE ${WRAPPED_MODULES})
	endif()
	add_metadata(WindowsIcon.ico)
	apply_compile_settings()

	if(NOT ANDROID)
		if(NOT DEFINED PROJECT_BUILD_CRYENGINE OR PROJECT_BUILD_CRYENGINE)
			install(
				TARGETS ${target}
				RUNTIME DESTINATION bin
				ARCHIVE DESTINATION lib)
		endif()
	endif()
endfunction()

function(DedicatedServer target)
	prepare_project(${ARGN})
	if(WINDOWS)
		add_executable(${THIS_PROJECT} WIN32 ${${THIS_PROJECT}_SOURCES})
		set_property(TARGET ${THIS_PROJECT} APPEND_STRING PROPERTY LINK_FLAGS " /SUBSYSTEM:WINDOWS")
	else()
		add_executable(${THIS_PROJECT} ${${THIS_PROJECT}_SOURCES})
	endif()
	set_property(TARGET ${THIS_PROJECT} PROPERTY OUTPUT_NAME "Game_Server")
	add_metadata(WindowsServerIcon.ico)

	if(OPTION_STATIC_LINKING)
		use_scaleform()
		target_compile_definitions(${THIS_PROJECT} PRIVATE _LIB -DCRY_IS_MONOLITHIC_BUILD)
		if (WINDOWS)
			set_property(TARGET ${THIS_PROJECT} APPEND_STRING PROPERTY LINK_FLAGS_PROFILE " /NODEFAULTLIB:libcpmt.lib")
			set_property(TARGET ${THIS_PROJECT} APPEND_STRING PROPERTY LINK_FLAGS_RELEASE " /NODEFAULTLIB:libcpmt.lib")
		endif()
		set(MODULES_LIST ${GAME_MODULES} ${MODULES})
		wrap_whole_archive(${target} WRAPPED_MODULES MODULES_LIST)
		target_link_libraries(${THIS_PROJECT} PRIVATE ${WRAPPED_MODULES})
	endif()

	if(OPTION_DEDICATED_SERVER) 
		target_compile_definitions( ${THIS_PROJECT} PRIVATE "-DDEDICATED_SERVER") 
	endif() 
	
	apply_compile_settings()	
endfunction()

function(CryFileContainer target)
	set(THIS_PROJECT
			${target}
			PARENT_SCOPE)
	set(THIS_PROJECT ${target})
	project(${target})

	read_settings(${ARGN})
	if(NOT ${THIS_PROJECT}_SOURCES)
		set(${THIS_PROJECT}_SOURCES ${SOURCES})
	endif()

	add_custom_target(${THIS_PROJECT} SOURCES ${${THIS_PROJECT}_SOURCES})
	if(MODULE_SOLUTION_FOLDER)
		set_solution_folder("${MODULE_SOLUTION_FOLDER}" ${THIS_PROJECT})
	endif()
endfunction()

# For Windows, an argument may be provided to specify the location of an icon
# for the executable
function(add_metadata)
	if(WINDOWS)
		get_target_property(project_type ${THIS_PROJECT} TYPE)
		set(valid_types EXECUTABLE MODULE_LIBRARY SHARED_LIBRARY)
		if(NOT ${project_type} IN_LIST valid_types)
			return()
		endif()

		set(icon_name ${ARGN})
		if(NOT PRODUCT_NAME)
			set(PRODUCT_NAME ${THIS_PROJECT})
		endif()
		file(
			WRITE "${CMAKE_CURRENT_BINARY_DIR}/${THIS_PROJECT}.autogen.rc"
			"// Microsoft Visual C++ generated resource script.\n"
			"//\n"
			"#include \"resource.h\"\n"
			"\n"
			"#define APSTUDIO_READONLY_SYMBOLS\n"
			"/////////////////////////////////////////////////////////////////////////////\n"
			"//\n"
			"// Generated from the TEXTINCLUDE 2 resource.\n"
			"//\n"
			"#include \"winres.h\"\n"
			"\n"
			"/////////////////////////////////////////////////////////////////////////////\n"
			"#undef APSTUDIO_READONLY_SYMBOLS\n"
			"\n")
		if(FALSE)
			file(
				APPEND "${CMAKE_CURRENT_BINARY_DIR}/${THIS_PROJECT}.autogen.rc"
				"/////////////////////////////////////////////////////////////////////////////\n"
				"// Neutral resources\n"
				"\n"
				"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_NEU)\n"
				"LANGUAGE LANG_NEUTRAL, SUBLANG_NEUTRAL\n"
				"#pragma code_page(1252)\n"
				"\n"
				"/////////////////////////////////////////////////////////////////////////////\n"
				"//\n"
				"// Cursor\n"
				"//\n"
				"\n"
				"\"${project.cursor_resource_name}\"   CURSOR                  \"${project.cursor_name}\"\n"
				"\n"
				"#endif    // Neutral resources\n"
				"/////////////////////////////////////////////////////////////////////////////\n"
			)
		endif()
		file(
			APPEND "${CMAKE_CURRENT_BINARY_DIR}/${THIS_PROJECT}.autogen.rc"
			"		"
			"/////////////////////////////////////////////////////////////////////////////\n"
			"// English (United States) resources\n"
			"\n"
			"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)\n"
			"LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US\n"
			"#pragma code_page(1252)\n"
			"\n"
			"#ifdef APSTUDIO_INVOKED\n"
			"/////////////////////////////////////////////////////////////////////////////\n"
			"//\n"
			"// TEXTINCLUDE\n"
			"//\n"
			"\n"
			"1 TEXTINCLUDE \n"
			"BEGIN\n"
			"    \"resource.h\\0\"\n"
			"END\n"
			"\n"
			"2 TEXTINCLUDE \n"
			"BEGIN\n"
			"    \"#include \"\"winres.h\"\"\\r\\n\"\n"
			"    \"\\0\"\n"
			"END\n"
			"\n"
			"3 TEXTINCLUDE \n"
			"BEGIN\n"
			"    \"\\r\\n\"\n"
			"    \"\\0\"\n"
			"END\n"
			"\n"
			"#endif    // APSTUDIO_INVOKED\n"
			"\n")
		if(icon_name AND EXISTS "${game_folder}/../Resources/${icon_name}")
			file(COPY "${game_folder}/../Resources/${icon_name}"
					 DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
			file(
				APPEND "${CMAKE_CURRENT_BINARY_DIR}/${THIS_PROJECT}.autogen.rc"
				"// Icon with lowest ID value placed first to ensure application icon\n"
				"// remains consistent on all systems.\n"
				"IDI_ICON                ICON                    \"${icon_name}\"\n")
		else()
			message(
				STATUS "icon [${game_folder}/../Resources/${icon_name}] not exists")
		endif()
		file(
			APPEND "${CMAKE_CURRENT_BINARY_DIR}/${THIS_PROJECT}.autogen.rc"
			"#endif    // English (United States) resources\n"
			"/////////////////////////////////////////////////////////////////////////////\n"
			"\n"
			"\n"
			"/////////////////////////////////////////////////////////////////////////////\n"
			"// German (Germany) resources\n"
			"\n"
			"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_DEU)\n"
			"LANGUAGE LANG_GERMAN, SUBLANG_GERMAN\n"
			"#pragma code_page(1252)\n"
			"\n"
			"/////////////////////////////////////////////////////////////////////////////\n"
			"//\n"
			"// Version\n"
			"//\n"
			"\n"
			"VS_VERSION_INFO VERSIONINFO\n"
			" FILEVERSION ${METADATA_VERSION_COMMA}\n"
			" PRODUCTVERSION ${METADATA_VERSION}\n"
			" FILEFLAGSMASK 0x17L\n"
			"#ifdef _DEBUG\n"
			" FILEFLAGS 0x1L\n"
			"#else\n"
			" FILEFLAGS 0x0L\n"
			"#endif\n"
			" FILEOS 0x4L\n"
			" FILETYPE 0x2L\n"
			" FILESUBTYPE 0x0L\n"
			"BEGIN\n"
			"    BLOCK \"StringFileInfo\"\n"
			"    BEGIN\n"
			"        BLOCK \"000904b0\"\n"
			"        BEGIN\n"
			"            VALUE \"FileDescription\", \"${METADATA_VERSION_COMMA}\"\n"
			"            VALUE \"CompanyName\", \"${METADATA_COMPANY}\"\n"
			"            VALUE \"FileVersion\", \"${METADATA_VERSION_COMMA}\"\n"
			"            VALUE \"LegalCopyright\", \"${METADATA_COPYRIGHT}\"\n"
			"            VALUE \"ProductName\", \"${PRODUCT_NAME}\"\n"
			"            VALUE \"ProductVersion\", \"${METADATA_VERSION}\"\n"
			"        END\n"
			"    END\n"
			"    BLOCK \"VarFileInfo\"\n"
			"    BEGIN\n"
			"        VALUE \"Translation\", 0x9, 1200\n"
			"    END\n"
			"END\n"
			"\n"
			"#endif    // German (Germany) resources\n"
			"/////////////////////////////////////////////////////////////////////////////\n"
			"\n"
			"\n"
			"\n"
			"#ifndef APSTUDIO_INVOKED\n"
			"/////////////////////////////////////////////////////////////////////////////\n"
			"//\n"
			"// Generated from the TEXTINCLUDE 3 resource.\n"
			"//\n"
			"\n"
			"\n"
			"/////////////////////////////////////////////////////////////////////////////\n"
			"#endif    // not APSTUDIO_INVOKED\n")
		target_sources(
			${THIS_PROJECT}
			PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/${THIS_PROJECT}.autogen.rc")
		source_group("Resource Files"
								 FILES "${CMAKE_CURRENT_BINARY_DIR}/${THIS_PROJECT}.autogen.rc")
	elseif(METADATA_VERSION)
		string(REPLACE "." ";" VERSION_LIST ${METADATA_VERSION})
		list(GET VERSION_LIST 0 VERSION_MAJOR)
		list(GET VERSION_LIST 1 VERSION_MINOR)
		list(GET VERSION_LIST 2 VERSION_REVISION)
		list(GET VERSION_LIST 3 VERSION_BUILD)

		target_compile_definitions(${THIS_PROJECT}
															 PRIVATE EXE_VERSION_INFO_0=${VERSION_MAJOR})
		target_compile_definitions(${THIS_PROJECT}
															 PRIVATE EXE_VERSION_INFO_1=${VERSION_MINOR})
		target_compile_definitions(${THIS_PROJECT}
															 PRIVATE EXE_VERSION_INFO_2=${VERSION_REVISION})
		target_compile_definitions(${THIS_PROJECT}
															 PRIVATE EXE_VERSION_INFO_3=${VERSION_BUILD})
	endif()
endfunction()

commonmacrosinit()
