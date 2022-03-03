start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Source Files"
		"pch.hpp"
		"resource.h"
		"../ResourceTools/ini.h"
		"../ResourceTools/ini.c"
		"ResourceCompiler.h"
		"ResourceCompiler.cpp"
		"ResourceCompilerPlugin.def"
		"FileMapping.cpp"
		"FileMapping.h"
)
end_sources()

CryResourceCompiler(ResourceCompiler PCH "pch.hpp" SOLUTION_FOLDER "RC")


