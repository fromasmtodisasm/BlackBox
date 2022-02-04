# ##############################################################################
start_sources()
sources_platform(ALL)
add_sources("NoUberFile" 
	SOURCE_GROUP "Source Files"
		"SDLWindow.cpp"
)

EngineModule(Window PCH "pch.hpp" SOLUTION_FOLDER "Engine")
target_compile_definitions(${THIS_PROJECT} PUBLIC -DSDL_WINDOW)
target_link_libraries(${THIS_PROJECT} PUBLIC Math)
add_sdl(${THIS_PROJECT})
# ##############################################################################
