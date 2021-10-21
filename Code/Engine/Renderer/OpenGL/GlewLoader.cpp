#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ConsoleRegistration.h>

using namespace std;

const char* interesting_exts[] = {
	"GL_ARB_shading_language_100",
	"GL_GOOGLE_include_directive",
	"GL_ARB_direct_state_access",
	"GL_ARB_spirv_extensions ",
	"GL_ARB_gl_spirv"
};


void print_supported(const char* ext)
{
	auto supported = glewIsSupported(ext);
	{
		GL_LOG("%s %s", ext, supported ? "supported" : " not supported");	
	}
}

void print_exts(const std::vector<std::string_view>& exts)
{
	for (auto ext : exts)
	{
		print_supported(ext.data());	
	}
}

void print_supported_cmd(IConsoleCmdArgs* args)
{
	std::vector<std::string_view> exts;
	for (int i = 1; i < args->GetArgCount(); i++)
	{
		exts.push_back(args->GetArg(i));	
	}
	print_exts(exts);
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

	REGISTER_COMMAND("gl_ext", print_supported_cmd, 0, "print if ext supported or not");
	
	std::vector<std::string_view> exts;
	for (auto ext : interesting_exts)
	{
		exts.push_back(ext);	
	}
	
	print_exts(exts);
  
  return true;
}
