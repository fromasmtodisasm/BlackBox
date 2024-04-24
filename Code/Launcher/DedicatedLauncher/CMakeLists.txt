set(CMAKE_INCLUDE_CURRENT_DIR ON)

start_sources()
# -- launcher.waf_files --
sources_platform(WINDOWS)
add_sources("NoUberFile"
	SOURCE_GROUP "Header Files"
		"pch.hpp"
	SOURCE_GROUP "Resource Files"
		"resource.h"
	SOURCE_GROUP "Source Files"
		"Main.cpp"
)
end_sources()

if(WINDOWS)
	DedicatedServer(DedicatedLauncher SOLUTION_FOLDER "Launcher" PCH "pch.hpp")
	target_link_libraries( ${THIS_PROJECT} PRIVATE Shell32)
	target_link_libraries(${THIS_PROJECT} PRIVATE  BlackBox)
else()
	CryFileContainer(DedicatedLauncher SOLUTION_FOLDER "Launcher")
endif()
