# ===============> bison
find_package(BISON)
find_package(FLEX)
find_package(Cygwin)

SET(CYGWIN_CMD_PREFIX ${CYGWIN_INSTALL_PATH}/bin/bash --login -c)

macro(cygwin_bison INPUT OUTPUT)
	#get_filename_component(INPUT_NAME ${INPUT} NAME)
	set(INTPUT_NAME ${INTPUT})
	add_custom_command(
		OUTPUT ${OUTPUT}
		COMMAND ${CYGWIN_CMD_PREFIX} "bison -v -Wother -Wcounterexamples -o `cygpath -u ${OUTPUT}` `cygpath -u ${INPUT}`"
		DEPENDS ${INPUT})
endmacro()

macro(cygwin_flex INPUT OUTPUT)
	#get_filename_component(INPUT_NAME ${INPUT} NAME)
	set(INTPUT_NAME ${INTPUT})
	add_custom_command(
		OUTPUT ${OUTPUT}
		COMMAND ${CYGWIN_CMD_PREFIX} "flex  -o `cygpath -u ${OUTPUT}` `cygpath -u ${INPUT}`"
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

##########################################
macro(wsl_bison INPUT OUTPUT)
    add_custom_command(
        OUTPUT ${OUTPUT}
        COMMAND wsl bison -v -Wother -Wcounterexamples -o `wslpath -a ${OUTPUT}` `wslpath -a ${INPUT}`
        DEPENDS ${INPUT}
    )
endmacro()

macro(wsl_flex INPUT OUTPUT)
    add_custom_command(
        OUTPUT ${OUTPUT}
        COMMAND wsl flex -o `wslpath -a ${OUTPUT}` `wslpath -a ${INPUT}`
        DEPENDS ${INPUT}
    )
endmacro()

macro(wsl_bison_target TARGET INPUT OUTPUT)
    wsl_bison(${INPUT} ${OUTPUT})
    add_custom_target(${TARGET} DEPENDS ${OUTPUT})
    set_target_properties(${TARGET} PROPERTIES FOLDER "FxParser")
endmacro()

macro(wsl_flex_target TARGET INPUT OUTPUT)
    wsl_flex(${INPUT} ${OUTPUT})
    add_custom_target(${TARGET} DEPENDS ${OUTPUT})
    set_target_properties(${TARGET} PROPERTIES FOLDER "FxParser")
endmacro()

macro(wsl_add_flex_bison_dependency FLEX_TARGET BISON_TARGET)
	add_dependencies(${THIS_PROJECT} ${FLEX_TARGET} ${BISON_TARGET})
endmacro()

macro(setup_bison_target Target Input Output)
    if(BISON_FOUND)
        bison_target(${Target} ${Input} ${Output})
    elseif(WSL_FOUND)
        wsl_bison_target(${Target} ${Input} ${Output})
    elseif(CYGWIN_INSTALL_PATH)
        cygwin_bison_target(${Target} ${Input} ${Output})
    endif()
endmacro()

macro(setup_flex_target Target Input Output)
    if(FLEX_FOUND)
        flex_target(${Target} ${Input} ${Output})
    elseif(WSL_FOUND)
        wsl_flex_target(${Target} ${Input} ${Output})
    elseif(CYGWIN_INSTALL_PATH)
        cygwin_flex_target(${Target} ${Input} ${Output})
    endif()
endmacro()

macro(setup_flex_bison_dependencies FlexTarget BisonTarget)
	if(FLEX_FOUND AND BISON_FOUND)
		add_flex_bison_dependency(${FlexTarget} ${BisonTarget})
	elseif(WSL_FOUND)
		wsl_add_flex_bison_dependency(${FlexTarget} ${BisonTarget})
	elseif(CYGWIN_INSTALL_PATH)
		cygwin_add_flex_bison_dependency(${FlexTarget} ${BisonTarget})
	endif()
endmacro()

# Try find WSL instance
function(check_wsl)
    execute_process(
        COMMAND cmd.exe /c "wsl -l -q"
        RESULT_VARIABLE result
        OUTPUT_QUIET
        ERROR_QUIET
    )
    if(result EQUAL 0)
        set(WSL_FOUND TRUE PARENT_SCOPE)
		message(STATUS "WSL found")
    else()
        set(WSL_FOUND FALSE PARENT_SCOPE)
    endif()
endfunction()

check_wsl()
