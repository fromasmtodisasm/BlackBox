start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	PROJECTS Utils
	SOURCE_GROUP "SourceFiles"
		"MusicList.cpp"
		"Utils.cpp"
 )
end_sources()

EngineModule(Utils FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER "Engine")
