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
  set(EXECUTE_COMMAND vcpkg install ${Package})
  message(STATUS "Trying install package [${Package}]")
  execute_process(
    COMMAND ${EXECUTE_COMMAND} 
    RESULT_VARIABLE result
  )
  set(${Result} ${result} PARENT_SCOPE)

endfunction()
