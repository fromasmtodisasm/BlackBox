set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

start_sources()
# -- launcher.waf_files --
sources_platform(WINDOWS)
add_sources(
	"NoUberFile"
	SOURCE_GROUP "Header Files"
		"pch.hpp"
	SOURCE_GROUP "Resource Files"
		"resource.h"
	SOURCE_GROUP "Source Files"
		"Main.cpp"
		"MainWindow.cpp"
		"MainWindow.hpp"
)
Editor(Sandbox PCH "pch.hpp" SOLUTION_FOLDER "Sandbox")

