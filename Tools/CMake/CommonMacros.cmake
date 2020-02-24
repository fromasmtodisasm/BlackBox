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
    COMMAND ${CMD} install "${ToInstall}"
    RESULT_VARIABLE result
  )
  set(${Result} ${result} PARENT_SCOPE)

endfunction()

macro(InstallVcpkg)
	find_package(Git)
	if (NOT GIT_FOUND)
		message(FATAL_ERROR "Git not found")
	else()
		message(STATUS "Git FOUND at ${GIT_EXECUTABLE}")
	endif()
endmacro()

macro(CommonMacrosInit)
	message(STATUS "INITIALIZE COMMON MACROS")
	if (DEFINED LINUX)
	  set(BB_PLATFORM linux)
	else()
	  set(BB_PLATFORM windows)
	endif()
	find_program(VCPKG NAMES vcpkg)
	if (VCPKG EQUAL NOTFOUND)
		message(FATAL_ERROR "vcpkg not found, var: ${VCPKG}")
	else()
		message(STATUS "VCPKG FOUND at [${VCPKG}]")
	endif()
	InstallVcpkg()
endmacro()

macro(add_package Package)
	message(STATUS "Adding ${Package}")
	find_package(${Package})
	if (NOT ${Package}_FOUND)
		message(STATUS "${Package} yet not installed")
		install_package("${Package}" result)
		if (NOT result EQUAL 0)
			message(FATAL_ERROR "could not install ${Package}") 
		endif()
		find_package(${Package} CONFIG)
		if (NOT ${Package}_FOUND)
			find_package(${Package} REQUIRED)

		endif()
	endif()
endmacro()

# Helper macro to set default StartUp Project in Visual Studio
macro(set_solution_startup_target target)
	if ("${CMAKE_GENERATOR}" MATCHES "^Visual Studio")
		# Set startup project to launch Game.exe with this project
		set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" PROPERTY VS_STARTUP_PROJECT ${target})
	endif()
endmacro()

macro(add_subsystem subsystem)
	add_subdirectory(${ENGINE_DIR}/${subsystem} ${subsystem})
	target_link_libraries(${BLACKBOX_PROJECT} PRIVATE ${subsystem})
	set_target_properties(${subsystem} PROPERTIES FOLDER "Engine")

  get_target_property(SOURCE_FILES ${subsystem} SOURCES)

  list(APPEND ALL_PROJECT_SOURCES ${SOURCE_FILES})

endmacro()

CommonMacrosInit()
