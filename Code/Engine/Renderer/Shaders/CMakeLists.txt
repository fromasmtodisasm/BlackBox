start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	FxParser
	SOURCE_GROUP
	"Source Files"
	"Parser.cpp"
	"Effect.cpp"
	"Scanner.cpp"
	"Driver.cpp"
	"Parser.yy"
	"Parser.hpp"
	"Scanner.ll"
	"dummy.cpp"
	"ScannerHelpers.cpp")
end_sources()

enginemodule(FxParser FORCE_SHARED_WIN PCH "./pch.hpp" SOLUTION_FOLDER "Engine")

# ===============> bison
find_package(BISON)
find_package(FLEX)
find_package(Cygwin)

macro(cygwin_bison INPUT OUTPUT)
	#get_filename_component(INPUT_NAME ${INPUT} NAME)
	set(INTPUT_NAME ${INTPUT})
	add_custom_command(
		OUTPUT ${OUTPUT}
		COMMAND ${CYGWIN_INSTALL_PATH}/bin/bash --login -c "bison  -o `cygpath -u ${OUTPUT}` `cygpath -u ${INPUT}`"
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
endmacro()
macro(cygwin_flex_target TARGET INPUT OUTPUT)
	cygwin_flex(${INPUT} ${OUTPUT})
	add_custom_target(${TARGET} DEPENDS ${INTPUT})
endmacro()
macro(cygwin_add_flex_bison_dependency FLEX_TARGET BISON_TARGET)
	add_dependencies(${THIS_PROJECT} ${FLEX_TARGET} ${BISON_TARGET})
endmacro()

macro(setup_bison_target)
	if(BISON_FOUND)
		bison_target(
			ShaderParser ${CMAKE_CURRENT_SOURCE_DIR}/Parser.yy
			${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
		)
	elseif(CYGWIN_INSTALL_PATH)
		cygwin_bison_target(
			ShaderParser ${CMAKE_CURRENT_SOURCE_DIR}/Parser.yy
			${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
		)
	endif()
endmacro()

macro(setup_flex_target)
	if(FLEX_FOUND)
		flex_target(ShaderScanner ${CMAKE_CURRENT_SOURCE_DIR}/Scanner.ll
								${CMAKE_CURRENT_SOURCE_DIR}/Scanner.cpp)
	elseif(CYGWIN_INSTALL_PATH)
		cygwin_flex_target(ShaderScanner ${CMAKE_CURRENT_SOURCE_DIR}/Scanner.ll
								${CMAKE_CURRENT_SOURCE_DIR}/Scanner.cpp)
	endif()
endmacro()

macro(setup_flex_bison_dependencies)
	if(FLEX_FOUND AND BISON_FOUND)
		add_flex_bison_dependency(ShaderScanner ShaderParser)
	else()
		cygwin_add_flex_bison_dependency(ShaderScanner ShaderParser)
	endif()
endmacro()

if((NOT BISON_FOUND) OR (NOT FLEX_FOUND))
	message(
		WARNING
			"
			No Bison and/or Flex available. FxParser will only use the already generated code.
			You can try to set BISON_EXECUTABLE & FLEX_EXECUTABLE")
	set(BISON_EXECUTABLE
			""
			CACHE PATHFILE "bison")
	set(FLEX_EXECUTABLE
			""
			CACHE PATHFILE "flex")
	mark_as_advanced(CLEAR BISON_EXECUTABLE)
	mark_as_advanced(CLEAR FLEX_EXECUTABLE)
endif()

setup_bison_target()
setup_flex_target()
setup_flex_bison_dependencies()

target_include_directories(FxParser PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(FxParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Shaders)
target_compile_definitions(FxParser PRIVATE "-DYY_NO_UNISTD_H")

target_link_libraries(FxParser PUBLIC Math)
target_include_directories(FxParser PRIVATE ${CMAKE_SOURCE_DIR})

install_this(FxParser)
