#pragma once
#ifdef GLAD_LOADER
#include <glad/glad.h>
#elif GLEW_LOADER
#include <GL/glew.h>
#else
#error OPENGL LOADER NOT SETTED
#endif

bool OpenGLLoader();
