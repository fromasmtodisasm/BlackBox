#include <BlackBox/Render/OpenglDebug.hpp>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <sstream>



OpenglDebug::OpenglDebug(const char *file) : debug_file(file)
{
	glDebugMessageCallback(callBack, &debug_file);
}

OpenglDebug::~OpenglDebug()
{

}

void OpenglDebug::callBack(GLenum source​, GLenum type​, GLuint id​, GLenum severity​, GLsizei length​, const GLchar* message​, const void* userParam​)
{
	//std::fstream* df = const_cast< std::fstream* > (reinterpret_cast< const std::fstream*​ >(userParam​) );

	std::stringstream ss;
	ss << SOURCE_TO_STRING(source​) << std::endl << TYPE_TO_STRING(type​) << std::endl << SEVERITY_TO_STRING(severity​) << std::endl << "Message: " << std::string(message​, message​ + length​) << std::endl 
		<< "end_msg\n";
	//df->write(ss.str().c_str(), ss.str().size());
	std::cout << ss.str().c_str() << std::endl;
}

