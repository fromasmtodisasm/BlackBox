macro(add_private_headers)
	file(GLOB_RECURSE INCLUDE_FILES ${BLACKBOX_PRIVATE_INCLUDE_DIRS}/${PROJECT_NAME}/*.hpp ${BLACKBOX_PRIVATE_INCLUDE_DIRS}/${PROJECT_NAME}/*.h)
	target_sources(${PROJECT_NAME}
		PRIVATE
		${INCLUDE_FILES}
	)
	source_group(
		TREE ${BLACKBOX_PRIVATE_INCLUDE_DIRS}/${PROJECT_NAME}
		PREFIX "Header Files"
		FILES
		${INCLUDE_FILES}
	)
endmacro()

function(install_package Package Result)

  #set(ToInstall "${Package}:${CMAKE_GENERATOR_PLATFORM}-${BB_PLATFORM}")
  set(ToInstall "${Package}:x64-${BB_PLATFORM}")
  set(CMD "vcpkg")
  set(ARGS " install ${ToInstall}")
  message(STATUS "Trying install package [${ToInstall}]")
  execute_process(
    COMMAND ${VCPKG} install "${ToInstall}"
    RESULT_VARIABLE result
  )
  set(${Result} ${result} PARENT_SCOPE)
endfunction()

macro(FindGit)
	find_package(Git)
	if (NOT GIT_FOUND)
		message(FATAL_ERROR "Git not found")
	else()
		message(STATUS "Git FOUND at ${GIT_EXECUTABLE}")
	endif()
endmacro()

macro(InstallVcpkg)
	find_program(VCPKG NAMES vcpkg PATHS ${CMAKE_SOURCE_DIR}/vcpkg)
	if (NOT VCPKG)
    include(${TOOLS_CMAKE_DIR}/PrepareVcpkg.cmake)
		PrepareVcpkg()
		find_program(VCPKG NAMES vcpkg)
		if (NOT VCPKG)
			message(FATAL_ERROR "vcpkg not found, var: ${VCPKG}")
		endif()
	else()
		message(STATUS "VCPKG FOUND at [${VCPKG}]")
	endif()
endmacro()

macro(CommonMacrosInit)
	message(STATUS "INITIALIZE COMMON MACROS")
  if ((DEFINED LINUX OR MINGW) AND (NOT DEFINED VCPKG_INSTALLER))
    include(FindPkgConfig)
    message("lskdjflskdjflsjflksdjfklsjlfkjsdlkfjkldsjf")
  elseif(DEFINED LINUX AND (DEFINED VCPKG_INSTALLER))
	  set(BB_PLATFORM linux)
	else()
	  set(BB_PLATFORM windows)
	endif()
	InstallVcpkg()
endmacro()

function(dump_vars)
	message(STATUS ==============)
	message(STATUS "Dumping ${ARGC} variables:")
	foreach(VAR_NAME ${ARGN})
		message(
		STATUS " ${VAR_NAME} == \"${${VAR_NAME}}\"")
	endforeach()
	message(STATUS ==============)
endfunction()

function(add_package_unix_or_cygwin Package)
	find_package(${ARGV})
endfunction()

function(add_package_msvc Package)
  if (NOT DEFINED VCPKG_INSTALLER)
    find_package(${Package} REQUIRED)
  else (NOT ${Package}_FOUND)
		set(result OFF)
		if (NOT ${Package}_INSTALLED)
			message(STATUS "${Package} yet not installed")
			install_package("${Package}" result)
			if (NOT result EQUAL 0)
				message(FATAL_ERROR "could not install ${Package}") 
			else()
				set(${Package}_INSTALLED ON CACHE STRING "Installed package" FORCE)
			endif()
		endif()
		find_package(${ARGV} CONFIG)
		if (NOT ${Package}_FOUND)
			find_package(${ARGV} REQUIRED)
		endif()
	endif()
endfunction()

function(add_package Package)
	find_package(${ARGV})
  if (0 AND (MINGW OR UNIX OR CYGWIN))
    add_package_unix_or_cygwin(${Package})
  else()
    add_package_msvc(${Package})
  endif()
endfunction()

# Helper macro to set default StartUp Project in Visual Studio
macro(set_solution_startup_target target)
	if ("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
		# Set startup project to launch Game.exe with this project
		set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${target})
	endif()
endmacro()

macro(add_subsystem subsystem)
	add_subdirectory(${ENGINE_DIR}/${subsystem} ${subsystem})
	target_link_libraries(${BLACKBOX_PROJECT} INTERFACE ${subsystem})
	set_target_properties(${subsystem} PROPERTIES FOLDER "Engine")

	get_target_property(SOURCE_FILES ${subsystem} SOURCES)

	list(APPEND ALL_PROJECT_SOURCES ${SOURCE_FILES})
	list(APPEND ALL_PROJECT_SYSTEMS ${subsystem})
	install_this(${subsystem})
endmacro()

macro(export_subsystem subsystem)

endmacro()

macro(install_this THIS_PROJECT)
	###################################################
	install (
	  TARGETS  ${THIS_PROJECT}
	  EXPORT ${THIS_PROJECT}Config
	  RUNTIME DESTINATION bin
	  #LIBRARY DESTINATION lib
	  ARCHIVE DESTINATION lib/static
	)

	# This makes the project importable from the install directory
	# Put config file in per-project dir (name MUST match), can also
	# just go into 'cmake'.
	install(EXPORT ${THIS_PROJECT}Config DESTINATION .)

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

function(add_sdl)
        if (DEFINED LINUX OR DEFINED MINGW)
                if (NOT DEFINED VCPKG_INSTALLER)
                        find_package(SDL2 CONFIG REQUIRED)
                        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
                        if(NOT SDL2_FOUND)
                        else()
                                target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARY})
                                target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})
                        endif()
		else()
                        add_package(SDL2 CONFIG REQUIRED)
                        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
		endif()
	else()
                add_package(SDL2 CONFIG REQUIRED)
		target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
	endif()
endfunction()

function(add_SDL_net)
	if (DEFINED LINUX OR DEFINED MINGW)
		if (NOT DEFINED VCPKG_INSTALLER)
                        pkg_check_modules(SDL2_NET SDL2_net)
			target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_NET_LIBRARY})
			target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_NET_INCLUDE_DIR})
		else()
                        add_package(SDL2-net)
                        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_net)
		endif()
	else()
		add_package(SDL2-net)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_net)
	endif()
endfunction()


CommonMacrosInit()
