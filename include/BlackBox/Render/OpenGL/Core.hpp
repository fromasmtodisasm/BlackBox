#pragma once
#ifdef GLAD_LOADER
//#define _WINDOWS_
#pragma warning(push)
#pragma warning(disable : 4005)
#include <glad/glad.h>
#pragma warning(pop)
#else
#ifdef GLEW_LOADER
#include <GL/glew.h>
#else
#error OPENGL LOADER NOT SETTED
#endif
#endif

#include <BlackBox/Render/OpenglDebug.hpp>

bool OpenGLLoader();
