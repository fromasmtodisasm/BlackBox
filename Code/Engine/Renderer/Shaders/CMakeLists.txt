start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Generated"
		${CMAKE_CURRENT_BINARY_DIR}/Parser.cpp
		${CMAKE_CURRENT_BINARY_DIR}/Scanner.cpp
		#${CMAKE_CURRENT_BINARY_DIR}/Parser.hpp
	SOURCE_GROUP "FlexBison"
		"Parser.yy"
		"Scanner.ll"
	SOURCE_GROUP "Source Files"
		"Driver.cpp"
		"Driver.hpp"
		"Effect.cpp"
		"Effect.hpp"
		"Scanner.hpp"
		"ScannerHelpers.cpp"
		"FxParser.h"
)
end_sources()

find_package(FlexBison)

setup_flex_target(ShaderScanner ${CMAKE_CURRENT_SOURCE_DIR}/Scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/Scanner.cpp)
setup_bison_target(ShaderParser ${CMAKE_CURRENT_SOURCE_DIR}/Parser.yy ${CMAKE_CURRENT_BINARY_DIR}/Parser.cpp)
setup_flex_bison_dependencies(ShaderScanner ShaderParser)

EngineModule(FxParser PCH "pch.hpp" SOLUTION_FOLDER "Engine")
target_compile_definitions(FxParser PUBLIC "-DYYDEBUG=1" "-DYY_EXCEPTIONS=0")


target_include_directories(FxParser 
	PRIVATE 
		${CMAKE_SOURCE_DIR}
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/Shaders
		${CMAKE_CURRENT_BINARY_DIR}
)
target_compile_definitions(FxParser PRIVATE "-DYY_NO_UNISTD_H")
target_link_libraries(FxParser PUBLIC Math)

install_this(FxParser)