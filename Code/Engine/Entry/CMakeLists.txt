start_sources()
sources_platform(ALL)
add_sources("NoUberFile" PROJECTS AppMain SOURCE_GROUP "Root"
						"Main.cpp")
end_sources()
EngineStaticModule(AppMain SOLUTION_FOLDER "Libs")

