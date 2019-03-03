#pragma once
#include <GL/glew.h>
#include <string>

class CShader 
{
private:
	GLuint m_shader;
	std::string path;
	std::string text;
public:
	enum type{
		E_VERTEX,
		E_FRAGMENT
	};
	CShader(std::string text, CShader::type type);
	static CShader *load(std::string path, CShader::type type);
	bool create();
	bool compile();
	bool bind();
};
