start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Root"
		"IEditor.h"
		"IViewportManager.h"
		"ILevelEditor.h"
)

end_sources()

CryFileContainer(EditorInterface SOLUTION_FOLDER Sandbox)