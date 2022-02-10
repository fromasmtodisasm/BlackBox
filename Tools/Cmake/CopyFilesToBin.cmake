macro(deploy_runtime_file source)
	if(${ARGC} GREATER 1)
		deploy_runtime_file_impl(${source} ${ARGV1})
	else()
		set_base_outdir()
		get_filename_component(FILE_NAME "${source}" NAME)
		# Ensure remainders of a generator expression get removed
		string(REPLACE ">" "" FILE_NAME "${FILE_NAME}")
		deploy_runtime_file_impl(${source} "${base_outdir}/${FILE_NAME}")
	endif()
endmacro()

macro(deploy_runtime_file_impl source destination)
	if(USE_CONFIG)
		# HACK: This works on the assumption that any individual file is only used by *one* configuration, or *all* configurations. CMake will generate errors otherwise.
		list(APPEND DEPLOY_FILES "$<$<CONFIG:${USE_CONFIG}>:${source}>")
		list(APPEND DEPLOY_FILES "${source}")
		list(APPEND DEPLOY_FILES "${destination}")
	else()
		list(APPEND DEPLOY_FILES "${source}")
		list(APPEND DEPLOY_FILES "${source}")
		list(APPEND DEPLOY_FILES "${destination}")
	endif()
	set(DEPLOY_FILES "${DEPLOY_FILES}" CACHE INTERNAL "List of files to deploy before running")
endmacro()

macro(set_base_outdir)
	set(base_outdir ${BASE_OUTPUT_DIRECTORY})
	if(
		(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio") AND
		(NOT ${CMAKE_GENERATOR} MATCHES "Ninja Multi-Config")
	)
		string( TOUPPER ${CMAKE_BUILD_TYPE} BUILD_CONFIG )
		set(base_outdir ${BASE_OUTPUT_DIRECTORY_${BUILD_CONFIG}})
	endif()
	if(OUTPUT_DIRECTORY_SUFFIX)
		set(base_outdir "${base_outdir}_${OUTPUT_DIRECTORY_SUFFIX}")
	endif()
	if(OPTION_DEDICATED_SERVER)
		set(base_outdir "${base_outdir}_dedicated")
	endif()
endmacro()

macro(deploy_runtime_files fileexpr)
	file(GLOB FILES_TO_COPY "${fileexpr}")
	foreach(FILE_PATH ${FILES_TO_COPY})
		get_filename_component(FILE_NAME "${FILE_PATH}" NAME)

		set_base_outdir()
		# If another argument was passed files are deployed to the subdirectory
		if (${ARGC} GREATER 1)
			deploy_runtime_file("${FILE_PATH}" "${base_outdir}/${ARGV1}/${FILE_NAME}")
			install(FILES "${FILE_PATH}" DESTINATION "bin/${ARGV1}")
		else ()
			deploy_runtime_file("${FILE_PATH}" "${base_outdir}/${FILE_NAME}")
			install(FILES "${FILE_PATH}" DESTINATION bin)
			if (TRUE)#((NOT USE_CONFIG))# AND (${CMAKE_GENERATOR} MATCHES "Visual Studio") AND (NOT source MATCHES "<"))
				# Make sure we also deploy to release directory for Visual Studio
				set(outdir_temp ${BASE_OUTPUT_DIRECTORY})
				#message(FATAL_ERROR "**********************************************")
				set(BASE_OUTPUT_DIRECTORY ${BASE_OUTPUT_DIRECTORY_RELEASE})
				set_base_outdir()
				#message(FATAL_ERROR "base_outdir is ${BASE_OUTPUT_DIRECTORY_RELEASE}")
				deploy_runtime_file("${FILE_PATH}" "${base_outdir}/${FILE_NAME}")
				set(BASE_OUTPUT_DIRECTORY ${outdir_temp})
			endif()
		endif ()
	endforeach()
endmacro()

macro(copy_binary_files_to_target)
	message( STATUS "copy_binary_files_to_target start ${BUILD_PLATFORM}" )
	set_base_outdir()

	if (OPTION_SANDBOX)
		# Qt: Debug && Profile will use the same win_x64 folder, for Release Qt will not be deployed. Empty second arg do the job.
		# d3dcompiler_47.dll file from Qt bin should not be copied in output folder
		#deploy_runtime_files("${QT_DEPLOY_ROOT}/bin/[^d][^3]*.dll" "")
		# second argument eliminate copy to release
		deploy_runtime_files("${QT_DEPLOY_ROOT}/bin/[^d][^3]*.dll")
		file(COPY ${QT_DEPLOY_ROOT}/plugins DESTINATION ${base_outdir})
		#qt6 not have this folder
		#file(COPY ${QT_DEPLOY_ROOT}/resources DESTINATION ${base_outdir})
	endif()

	if(DEPLOY_FILES)
		set(DEPLOY_DESTINATIONS)

		list(LENGTH DEPLOY_FILES deployFilesCount)
		math(EXPR idxRangeEnd "${deployFilesCount} - 1")
		foreach(idx RANGE 0 ${idxRangeEnd} 3)
			math(EXPR idxIncr "${idx} + 1")
			math(EXPR idxIncr2 "${idx} + 2")
			list(GET DEPLOY_FILES ${idx} source)
			list(GET DEPLOY_FILES ${idxIncr} source_file)
			list(GET DEPLOY_FILES ${idxIncr2} destination)

			if(source MATCHES "<") # Source contains generator expression; deploy at build time
				add_custom_command(OUTPUT ${destination}
					COMMAND ${CMAKE_COMMAND} "-DSOURCE=\"${source}\"" "-DDESTINATION=\"${destination}\"" -P "${TOOLS_CMAKE_DIR}/deploy_runtime_files.cmake"
					COMMENT "Deploying ${source_file}"
					DEPENDS "${source}")
				list(APPEND DEPLOY_DESTINATIONS "${destination}")
			else()
				message(STATUS "Deploying ${source_file} to ${destination}")
				get_filename_component(DEST_DIR "${destination}" DIRECTORY)
				file(COPY "${source}" DESTINATION "${DEST_DIR}" NO_SOURCE_PERMISSIONS)
			endif()

		endforeach(idx)

		if (DEPLOY_DESTINATIONS)
			add_custom_target(deployrt ALL DEPENDS ${DEPLOY_DESTINATIONS})
			set_solution_folder("Deploy" deployrt)
		endif()
	endif()
	message( STATUS "copy_binary_files_to_target end" )
endmacro()
