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
option(OPTION_USE_GUI "Build GUI library" ON)
option(OPTION_USE_LINUXINPUT "Build GUI library" ON)
option(OPTION_GLAD_LOADER "Build with GLAD opengl loading library" OFF)
option(OPTION_GLEW_LOADER "Build with GLEW opengl loading library" ON)
option(OPTION_EDITOR "Build editor" OFF)

option(OPTION_BUILD_TOOLS "Build Tools" ON)

option(OPTION_VCPKG "Use vcpkg for libraries" ON)

option(OPTION_GL_RENDERER "Compile OpenGL library" OFF)
option(OPTION_DX_RENDERER "Compile D3D library" OFF)
option(OPTION_VK_RENDERER "Compile Vulkan library" OFF)

option(OPTION_EDIT_AND_CONTINUE "Edit and continue MSVC feature" ON)

if(ANDROID)
	set(OPTION_STATIC_LINKING TRUE)
else()
	option(OPTION_STATIC_LINKING "Link all CryEngine modules as static libs to single exe" OFF)
endif()

if (OPTION_CONFIGURATOR OR OPTION_EDITOR)
	set(OPTION_USE_GUI ON)
	option(OPTION_QT_DIR "Qt Sdk Dir" "")
endif()
