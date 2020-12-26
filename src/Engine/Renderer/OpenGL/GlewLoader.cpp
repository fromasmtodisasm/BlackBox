#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <iostream>
using namespace std;

const char* interesting_exts[] = {
	"GL_ARB_shading_language_100",
	"GL_GOOGLE_include_directive",
	"GL_ARB_direct_state_access"
};


void print_supported(const char* ext)
{
	auto supported = glewIsSupported(ext);
	{
		GL_LOG("%s %s", ext, supported ? "supported" : " not supported");	
	}
}

void print_exts()
{
	for (auto ext : interesting_exts)
	{
		print_supported(ext);	
	}
}

bool OpenGLLoader()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		GL_LOG("Error: %s", glewGetErrorString(err));
		return false;
	}
	GL_LOG("Status: Using GLEW %s", glewGetString(GLEW_VERSION));
	
	print_exts();
  
  return true;
}
