option(PLUGIN_SAMPLE_EDITOR_PLUGIN "Build SamplePlugin" OFF)

set(EDITOR_DIR "Code/Sandbox/Editor" )
add_subdirectory("Code/Sandbox/Gui")
add_subdirectory("Code/Sandbox/Editor")
#add_subdirectory("Code/Sandbox/EditorCS")
add_subdirectory("Code/Sandbox/EditorQt")
add_subdirectory("Code/Sandbox/Plugins/EditorConsole")

add_subdirectory("Code/Sandbox/Plugins/EditorCommon")
add_subdirectory("Code/Sandbox/EditorInterface")
