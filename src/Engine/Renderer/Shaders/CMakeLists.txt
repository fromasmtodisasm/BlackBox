start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	PROJECTS FxParser
	SOURCE_GROUP "SourceFiles"
		"Parser.cpp"
		"Effect.cpp"
		"Scanner.cpp"
		"Driver.cpp"
		"Parser.yy"
		"Scanner.ll"
		"dummy.cpp"
		"ScannerHelpers.cpp"
 )
 end_sources()

EngineModule(FxParser FORCE_SHARED_WIN PCH "../pch.hpp" SOLUTION_FOLDER "Engine")

target_include_directories(FxParser PRIVATE ${CMAKE_SOURCE_DIR})
target_include_directories(FxParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Shaders)
target_compile_definitions(FxParser PRIVATE "-DYY_NO_UNISTD_H")

target_link_libraries(FxParser PUBLIC Math)
target_include_directories(FxParser PRIVATE ${CMAKE_SOURCE_DIR})

INSTALL_THIS(FxParser)

#target_precompile_headers(FxParser
#  PRIVATE 
#  "pch.hpp"
#)
