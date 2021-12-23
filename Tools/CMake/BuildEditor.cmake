option(PLUGIN_SAMPLE_EDITOR_PLUGIN "Build SamplePlugin" OFF)

set(EDITOR_DIR "Code/Sandbox/Editor" )
add_subdirectory("Code/Sandbox/Editor")
add_subdirectory("Code/Sandbox/Plugins/EditorConsole")

add_subdirectory("Code/Sandbox/Plugins/EditorCommon")
add_subdirectory("Code/Sandbox/EditorInterface")