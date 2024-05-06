# ##############################################################################
sources_platform(ALL)
start_sources()
add_sources(
	"NoUberFile"
	SOURCE_GROUP "PCH"
		"pch.hpp"
	SOURCE_GROUP "Source Files"
		"CryCharInstance.cpp"
		"CryCharInstance.h"
		"CryCharManager.cpp"
		"CryCharManager.h"
		"Main.cpp"
		
)
end_sources()
EngineModule(Animation PCH "pch.hpp" SOLUTION_FOLDER "Engine")

target_link_libraries(${THIS_PROJECT} PRIVATE Math)
