# ##############################################################################
set(INCLUDE_ROOT ${CMAKE_CURRENT_LIST_DIR}/../include)
# ##############################################################################

start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	TestRenderer
	SOURCE_GROUP "Source Files"
		Main.cpp
		TestRenderer.cpp
)
end_sources()
GameModule(TestRenderer FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER "Projects/Games")

target_link_libraries(${THIS_PROJECT} PRIVATE BlackBox)
if(OPTION_USE_GUI)
	add_compile_definitions("USE_GUI")
	target_link_libraries(${THIS_PROJECT} PRIVATE GUI)
	target_link_libraries(${THIS_PROJECT} PRIVATE ImGuiFileBrowser)
	target_link_libraries(${THIS_PROJECT} PRIVATE ImGuiColorTextEdit)
endif()

file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_ROOT}/*.hpp ${INCLUDE_ROOT}/*.h)

target_sources(${THIS_PROJECT} PRIVATE ${INCLUDE_FILES})

source_group(
	TREE ${INCLUDE_ROOT}
	PREFIX "Header Files"
	FILES ${INCLUDE_FILES})
