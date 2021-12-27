cmake_minimum_required(VERSION 3.10)

start_sources()
sources_platform(ALL)
add_sources("NoUberFile" 
	SOURCE_GROUP "Root"
		"Main.cpp"
)
end_sources()

EngineStaticModule(Entry SOLUTION_FOLDER "Libs")

