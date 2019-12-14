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

  set(ToInstall "${Package}:${CMAKE_GENERATOR_PLATFORM}-${BB_PLATFORM}")
  set(CMD "vcpkg")
  set(ARGS " install ${ToInstall}")
  message(STATUS "Trying install package [${ToInstall}]")
  execute_process(
    COMMAND ${CMD} install "${Package}:${CMAKE_GENERATOR_PLATFORM}-${BB_PLATFORM}"
    RESULT_VARIABLE result
  )
  set(${Result} ${result} PARENT_SCOPE)

endfunction()

macro(CommonMacrosInit)
  set(BB_PLATFORM windows)
endmacro()

CommonMacrosInit()
