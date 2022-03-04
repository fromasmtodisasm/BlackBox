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

CryResourceCompilerModule(ResourceCompilerPC PCH "pch.hpp" SOLUTION_FOLDER "BLACKBOX/Tools/RC")
target_include_directories(${THIS_PROJECT} PRIVATE
	"${CMAKE_SOURCE_DIR}/ResourceCompiler"
	#"${CRYENGINE_SOURCE_DIR}/Code/Tools"
	#"${CRYENGINE_SOURCE_DIR}/Code/Tools/CryCommonTools"
	#"${CRYENGINE_SOURCE_DIR}/Code/CryEngine/Cry3DEngine"
	#"${CRYENGINE_SOURCE_DIR}/Code/CryEngine/Cry3DEngine/MeshCompiler"
	#"${CRYENGINE_SOURCE_DIR}/Code/Sandbox"
	#"${SDK_DIR}/TangentBasisComputation"
	#"${SDK_DIR}"
	#"${SDK_DIR}/Alembic/alembic-1.1.0/include"
	#"${SDK_DIR}/Alembic/ilmbase-1.0.2/include"
)
target_compile_options(${THIS_PROJECT} PRIVATE /EHsc /GR)
target_compile_definitions(${THIS_PROJECT} PRIVATE -DCRYTOOLS -DRESOURCECOMPILERPC_EXPORTS -DOFFLINE_COMPUTATION)
if (OPTION_FBX_SDK)
	use_fbx_sdk()
endif()
target_link_libraries(${THIS_PROJECT} PRIVATE Gdi32 Shell32 Ole32 dbghelp Wininet)
#target_link_libraries(${THIS_PROJECT} PRIVATE lua)

