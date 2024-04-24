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

if(WINDOWS AND NOT OPTION_DEDICATED_SERVER)
	Launcher(WindowsLauncher SOLUTION_FOLDER "Launcher")
	target_link_libraries(${THIS_PROJECT} PRIVATE Shell32)
	target_link_libraries(${THIS_PROJECT} PRIVATE  BlackBox)
else()
	CryFileContainer(WindowsLauncher SOLUTION_FOLDER "Launcher")
endif()
