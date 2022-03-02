start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Source Files"
		"pch.hpp"
		"resource.h"
		"ResourceCompiler.h"
		"ResourceCompiler.cpp"
		"ResourceCompilerPlugin.def"
)
end_sources()

CryResourceCompilerModule(ResourceCompilerImage PCH "pch.hpp" SOLUTION_FOLDER "RC")
