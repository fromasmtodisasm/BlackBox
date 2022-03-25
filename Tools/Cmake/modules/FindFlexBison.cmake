# ===============> bison
find_package(BISON)
find_package(FLEX)
find_package(Cygwin)

macro(cygwin_bison INPUT OUTPUT)
	#get_filename_component(INPUT_NAME ${INPUT} NAME)
	set(INTPUT_NAME ${INTPUT})
	add_custom_command(
		OUTPUT ${OUTPUT}
		COMMAND ${CYGWIN_INSTALL_PATH}/bin/bash --login -c "bison -v -Wother -Wcounterexamples -o `cygpath -u ${OUTPUT}` `cygpath -u ${INPUT}`"
		DEPENDS ${INPUT})
endmacro()

macro(cygwin_flex INPUT OUTPUT)
	#get_filename_component(INPUT_NAME ${INPUT} NAME)
	set(INTPUT_NAME ${INTPUT})
	add_custom_command(
		OUTPUT ${OUTPUT}
		COMMAND ${CYGWIN_INSTALL_PATH}/bin/bash --login -c "flex  -o `cygpath -u ${OUTPUT}` `cygpath -u ${INPUT}`"
		DEPENDS ${INPUT})
endmacro()

macro(cygwin_bison_target TARGET INPUT OUTPUT)
	cygwin_bison(${INPUT} ${OUTPUT})
	add_custom_target(${TARGET} DEPENDS ${INTPUT})
	set_solution_folder(FxParser ${TARGET})
endmacro()
macro(cygwin_flex_target TARGET INPUT OUTPUT)
	cygwin_flex(${INPUT} ${OUTPUT})
	add_custom_target(${TARGET} DEPENDS ${INTPUT})
	set_solution_folder(FxParser ${TARGET})
endmacro()
macro(cygwin_add_flex_bison_dependency FLEX_TARGET BISON_TARGET)
	add_dependencies(${THIS_PROJECT} ${FLEX_TARGET} ${BISON_TARGET})
endmacro()

macro(setup_bison_target Target Input Output)
	if(BISON_FOUND)
		bison_target(${Target} ${Input} ${Output})
	elseif(CYGWIN_INSTALL_PATH)
		cygwin_bison_target(${Target} ${Input} ${Output})
	endif()
endmacro()

macro(setup_flex_target Target Input Output)
	if(FLEX_FOUND)
		flex_target(${Target} ${Input} ${Output})
	elseif(CYGWIN_INSTALL_PATH)
		cygwin_flex_target(${Target} ${Input} ${Output})
	endif()
endmacro()

macro(setup_flex_bison_dependencies FlexTarget BisonTarget)
	if(FLEX_FOUND AND BISON_FOUND)
		add_flex_bison_dependency(${FlexTarget} ${BisonTarget})
	elseif(CYGWIN_INSTALL_PATH)
		cygwin_add_flex_bison_dependency(${FlexTarget} ${BisonTarget})
	endif()
endmacro()

