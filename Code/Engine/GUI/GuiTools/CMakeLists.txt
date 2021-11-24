add_compile_definitions("_HAS_EXCEPTIONS=0")
add_subdirectory(ImGuiColorTextEdit)
add_subdirectory(ImGuiFileBrowser)

set_target_properties(ImGuiColorTextEdit PROPERTIES FOLDER "Engine/Utils/GUI")
set_target_properties(ImGuiFileBrowser PROPERTIES FOLDER "Engine/Utils/GUI")

