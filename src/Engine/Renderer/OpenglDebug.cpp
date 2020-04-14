#include <BlackBox/Renderer/OpenGL/Debug.hpp>

#include <iostream>
#include <vector>
#include <sstream>

bool OpenglDebuger::isError = false;
bool OpenglDebuger::ignore = false;

OpenglDebuger::OpenglDebuger(const char* file) : debug_file(file)
{
  if (glDebugMessageCallback != nullptr)
  {
		debug_file << "Creating Opengl Debugger" << std::endl;
    glDebugMessageCallback(callBack, &debug_file);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
		/*
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, NULL, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, NULL, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, NULL, GL_TRUE);
		*/

		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 0, GL_DEBUG_SEVERITY_HIGH, -1, "--Creating Opengl Debugger");
  }
}

OpenglDebuger::~OpenglDebuger()
{
	debug_file.close();
}

void OpenglDebuger::checkError(const char* file, int line, const char* expr)
{
	std::stringstream ss;
  if (isError/* && !ignore*/)
  {
    isError = false;

    // Log the error
    ss << "An internal OpenGL call failed in "
      << file << "(" << line << ")."
      << "\nExpression:\n   " << expr
      << "\n-*-*-*-*-*"
      << std::endl;
		std::cerr << ss.str();
  }
}

void OpenglDebuger::callBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
  std::ostream* df = const_cast< std::ostream* > (reinterpret_cast< const std::ostream* >(userParam) );

  //if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
  //	;// isError = false;
  //else
  //if (!ignore)
  //{
    isError = true;
    std::stringstream ss;
    ss << SOURCE_TO_STRING(source) << std::endl << TYPE_TO_STRING(type) << std::endl << SEVERITY_TO_STRING(severity) << std::endl << "Message: " << std::string(message, message + length);
    std::cerr << ss.str().c_str() << std::endl << std::endl;
		*df << ss.str();
  //}
}
