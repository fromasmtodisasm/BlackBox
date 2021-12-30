add_sources(
	"NoUberFile"
	SOURCE_GROUP "Code"
		"ConsoleWindow.cpp"
		"ConsoleWindow.h"

		"ConsolePlugin.h"
		"ConsolePlugin.cpp"
)
end_sources()
EditorPlugin(EditorConsole PCH "pch.hpp" SOLUTION_FOLDER "Sandbox/Plugins")

# _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING define is needed as this module makes use of boost and as of version 1.68.0 this warning still appears.
#target_compile_definitions( ${THIS_PROJECT} PRIVATE
#	-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#)
