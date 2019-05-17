#pragma once
#ifdef GLAD_LOADER
#include <glad/glad.h>
#else
#ifdef GLEW_LOADER
#include <GL/glew.h>
#else
#error OPENGL LOADER NOT SETTED
#endif
#endif

bool OpenGLLoader();
