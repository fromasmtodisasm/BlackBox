function(PrepareVcpkg)
  message("Setup Prepare VCPKG")
	#set(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE STRING "VCPKG_ROOT" FORCE)
	set(VCPKG "")
	if (VCPKG_ROOT)
		message(STATUS "Finding vcpkg in ${VCPKG_ROOT}")
		find_program(VCPKG NAMES vcpkg vcpkg.exe PATHS ${VCPKG_ROOT})
		message(STATUS "vcpkg is: ${VCPKG}")
	endif()
	if (NOT VCPKG)
		message(STATUS "VCPKG NOT FOUND")
		if (NOT (IS_DIRECTORY ${VCPKG_ROOT}/.git))
			include(FetchContent)
			FetchContent_Declare(
			  vcpkg  
			  GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
			  GIT_TAG        2021.05.12
			  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/Solutions/vcpkg
			)

			message(STATUS "Download VCPKG")
			FetchContent_MakeAvailable(vcpkg)
			set(VCPKG_ROOT ${vcpkg_SOURCE_DIR} CACHE STRING "VCPKG_ROOT" FORCE)
			message(STATUS "VCPKG_ROOT: ${VCPKG_ROOT}")
		endif()
		if(NOT (EXISTS ${VCPKG_ROOT}/vcpkg OR EXISTS ${VCPKG_ROOT}/vcpkg.exe))
			message(STATUS "Compiling vcpkg")
			set(FEXT bat)
			if (UNIX)
				set(FEXT sh)
			endif()
			execute_process(COMMAND ${VCPKG_ROOT}/bootstrap-vcpkg.${FEXT} WORKING_DIRECTORY ${VCPKG_ROOT} RESULT_VARIABLE RESULT)
			if (NOT ${RESULT} EQUAL 0)
				message(FATAL_ERROR "Could not compile vcpkg")
			endif()
		endif()
	endif()
	set(VCPKG ${VCPKG_ROOT}/vcpkg CACHE FILEPATH "VCPKG EXECUTABLE" FORCE)
	set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "CMake toolchain file" FORCE)
endfunction()

