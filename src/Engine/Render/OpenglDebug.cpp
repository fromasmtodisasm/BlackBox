#include <BlackBox/Render/OpenglDebug.hpp>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <sstream>

bool OpenglDebug::isError = false;

OpenglDebug::OpenglDebug(const char *file) : debug_file(file)
{
	glDebugMessageCallback(callBack, &debug_file);
}

OpenglDebug::~OpenglDebug()
{

}

void OpenglDebug::checkError(const char *file, int line, const char *expr)
{
	if (isError)
	{
		isError = false;

		// Log the error
		std::cerr << "An internal OpenGL call failed in "
			<< file << "(" << line << ")."
			<< "\nExpression:\n   " << expr
			<< "\n-*-*-*-*-*" 
			<< std::endl;
	}
}

void OpenglDebug::callBack(GLenum source​, GLenum type​, GLuint id​, GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam​)
{
	//std::fstream* df = const_cast< std::fstream* > (reinterpret_cast< const std::fstream*​ >(userParam​) );

	if (severity​ != GL_DEBUG_SEVERITY_NOTIFICATION)
		isError = true;
	else
		isError = false;
	std::stringstream ss;
	ss << SOURCE_TO_STRING(source​) << std::endl << TYPE_TO_STRING(type​) << std::endl << SEVERITY_TO_STRING(severity​) << std::endl << "Message: " << std::string(message​, message​ + length​);
	std::cerr << ss.str().c_str() << std::endl << std::endl;

}

