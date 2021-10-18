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
	add_custom_command(
		OUTPUT ${OUTPUT}
		COMMAND ${CYGWIN_INSTALL_PATH}/bin/bash --login -c "bison `cygpath -u ${INPUT}` -o `cygpath -u ${OUTPUT}`"
		DEPENDS ${INPUT})
endmacro()

macro(cygwin_flex INPUT OUTPUT)
	add_custom_command(
		OUTPUT ${OUTPUT}
		COMMAND ${CYGWIN_INSTALL_PATH}/bin/bash --login -c "flex `cygpath -u ${INPUT}` -o `cygpath -u ${OUTPUT}`"
		DEPENDS ${INPUT})
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
if(BISON_FOUND)
	bison_target(
		ShaderParser ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Parser.yy
		${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Parser.cpp
		# DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.hpp
	)
elseif(CYGWIN_INSTALL_PATH)
	cygwin_bison(${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Parser.yy
							 ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Parser.cpp)
endif()
if(FLEX_FOUND)
	flex_target(ShaderScanner ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Scanner.ll
							${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Scanner.cpp)
elseif(CYGWIN_INSTALL_PATH)
	message(STATUS "CYGWIN_INSTALL_PATH: ${CYGWIN_INSTALL_PATH}")
	cygwin_flex(${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Scanner.ll
							${CMAKE_CURRENT_SOURCE_DIR}/Shaders/Scanner.cpp)
endif()
if(FLEX_FOUND AND BISON_FOUND)
	add_flex_bison_dependency(ShaderScanner ShaderParser)
endif()

target_include_directories(FxParser PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(FxParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Shaders)
target_compile_definitions(FxParser PRIVATE "-DYY_NO_UNISTD_H")

target_link_libraries(FxParser PUBLIC Math)
target_include_directories(FxParser PRIVATE ${CMAKE_SOURCE_DIR})

install_this(FxParser)

# target_precompile_headers(FxParser PRIVATE "pch.hpp" )
