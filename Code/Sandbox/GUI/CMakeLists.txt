start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	GUI
	SOURCE_GROUP "Source Files"
		Console.cpp
		Console.h
		GUI.cpp
		ImGuiInput.cpp
		ImGuiInput.hpp
		ImGuiManager.cpp
		ImGuiManager.hpp
		ImGuiRenderer.cpp
		ImGuiRenderer.hpp
		ImGuiVanilaManager.cpp
		ImGuiVanilaManager.h
		MemoryEditor.h
		imconfig.h
		imgui.cpp
		imgui.h
		imgui_demo.cpp
		imgui_draw.cpp
		# already included in other file, skip it
		# imgui_impl_sdl.cpp
		imgui_impl_sdl.h
		imgui_internal.h
		imgui_tables.cpp
		imgui_widgets.cpp
		imstb_rectpack.h
		imstb_textedit.h
		imstb_true.h
		imstb_truetype.h
)
end_sources()

EngineModule(GUI FORCE_STATIC EDITOR_COMPILE_SETTINGS PCH "pch.hpp" SOLUTION_FOLDER "Sandbox")

target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Window)

target_include_directories(
${THIS_PROJECT}
	PRIVATE	"${CMAKE_BINARY_DIR}"
	PRIVATE	"${CMAKE_SOURCE_DIR}/"
	PUBLIC	${CMAKE_CURRENT_SOURCE_DIR}
)

#target_link_libraries(${THIS_PROJECT} PRIVATE Renderer)
#target_compile_definitions(GUI PRIVATE IMGUI_EXPORTS)
# FIXME: work around input system
add_sdl(${THIS_PROJECT})

add_subdirectory(GuiTools)
#add_private_headers()

add_library(imgui::imgui ALIAS GUI)
