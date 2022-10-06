function(PrepareVcpkg)
	if (NOT DOWNLOAD_VCPKG)
		message(STATUS "Skiping vcpkg prepare")
		return()
	endif()
	message(STATUS "Prepare VCPKG")
	if (DEFINED ENV{VCPKG_ROOT})
		if (NOT DEFINED VCPKG_ROOT)
			message(FATAL_ERROR "VCPKG_ROOT not defined")
			set(VCPKG_ROOT ENV{VCPKG_ROOT})
		endif()
	endif()
	if (NOT VCPKG_ROOT)
		message(STATUS ${BLACKBOX_DIR}/vcpkg)
		set(VCPKG_ROOT "${BLACKBOX_DIR}/vcpkg" CACHE FILEPATH "VCPKG_ROOT" FORCE)
	endif()

	message(STATUS "VCPKG_ROOT = ${VCPKG_ROOT}")

	find_program(VCPKG 
		NAMES 
			vcpkg vcpkg.exe 
		PATHS 
			${VCPKG_ROOT} ${BLACKBOX_DIR}
		HINTS 
			${VCPKG_ROOT}
		PATH_SUFFIXES
			vcpkg
		NO_CACHE
		NO_CMAKE_ENVIRONMENT_PATH
		NO_SYSTEM_ENVIRONMENT_PATH
		)
	message(STATUS "VCPKG = ${VCPKG}")

	if (NOT VCPKG)
		message(STATUS "VCPKG NOT FOUND")
		if (NOT (IS_DIRECTORY ${VCPKG_ROOT}/.git))
			include(FetchContent)
			FetchContent_Declare(
			  vcpkg  
			  GIT_REPOSITORY https://github.com/fromasmtodisasm/vcpkg.git
			  GIT_TAG        2022.08.15
			  SOURCE_DIR     ${BLACKBOX_DIR}/vcpkg
			)

			message(STATUS "Download VCPKG")
			FetchContent_MakeAvailable(vcpkg)
			set(VCPKG_ROOT ${vcpkg_SOURCE_DIR} CACHE STRING "VCPKG_ROOT" FORCE)
			message(STATUS "VCPKG_ROOT: ${VCPKG_ROOT}")
		endif()
	endif()
	#set(VCPKG ${VCPKG_ROOT}/vcpkg CACHE FILEPATH "VCPKG EXECUTABLE" FORCE)
	set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "CMake toolchain file" FORCE)

	message(STATUS "VCPKG = ${VCPKG}")
	message(STATUS "CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endfunction()
