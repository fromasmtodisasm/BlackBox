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

CryResourceCompiler(ResourceCompiler PCH "pch.hpp" SOLUTION_FOLDER "BLACKBOX/Tools/RC")
target_include_directories(${THIS_PROJECT} PRIVATE
	"${CMAKE_SOURCE_DIR}/ResourceCompiler"
	"${BLACKBOX_SOURCE_DIR}/Code/Tools"
	#"${BLACKBOX_SOURCE_DIR}/Code/Tools/CryCommonTools"
	#"${BLACKBOX_SOURCE_DIR}/Code/CryEngine/Cry3DEngine"
	#"${BLACKBOX_SOURCE_DIR}/Code/CryEngine/Cry3DEngine/MeshCompiler"
	"${BLACKBOX_SOURCE_DIR}/Code/Sandbox"
	#"${SDK_DIR}/TangentBasisComputation"
	#"${SDK_DIR}"
	#"${SDK_DIR}/Alembic/alembic-1.1.0/include"
	#"${SDK_DIR}/Alembic/ilmbase-1.0.2/include"
)
target_compile_options(${THIS_PROJECT} PRIVATE /EHsc /GR)
target_compile_definitions(${THIS_PROJECT} PRIVATE -DCRYTOOLS -DRESOURCECOMPILERPC_EXPORTS -DOFFLINE_COMPUTATION)


