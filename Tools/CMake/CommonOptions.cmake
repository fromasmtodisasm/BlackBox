if (NOT PROJECT_BUILD)
	option(OPTION_ENGINE "Build BLACKBOX" ON)
else()
	option(OPTION_ENGINE "Build BLACKBOX" OFF)
endif()

if (WINDOWS OR LINUX)
	option(OPTION_DEDICATED_SERVER "Build engine in Dedicated Server mode" OFF)
endif()

# Window managment
option(OPTION_USE_SDL "Build with SDL" ON)
option(OPTION_USE_SFML "Build with SFML" OFF)
option(OPTION_USE_GLFW "Build with GLFW" OFF)
option(OPTION_USE_GLM "Build with GLM Math library" ON)
option(OPTION_GLAD_LOADER "Build with GLAD opengl loading library" ON)

if(ANDROID)
	set(OPTION_STATIC_LINKING TRUE)
else()
	option(OPTION_STATIC_LINKING "Link all CryEngine modules as static libs to single exe" ON)
endif()
