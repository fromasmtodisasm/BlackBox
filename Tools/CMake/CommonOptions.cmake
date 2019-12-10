if (NOT PROJECT_BUILD)
	option(OPTION_ENGINE "Build BLACKBOX" ON)
else()
	option(OPTION_ENGINE "Build BLACKBOX" OFF)
endif()

if (WINDOWS OR LINUX)
	option(OPTION_DEDICATED_SERVER "Build engine in Dedicated Server mode" OFF)
endif()
