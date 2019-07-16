#include <BlackBox\Render\ScreenShader.hpp>

const char *vs = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
    TexCoords = aTexCoords;
}  
)";
const char *fs = R"(
#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    FragColor = texture(screenTexture, TexCoords);
}
)";

ScreenShader::ScreenShader()
	:
	CShaderProgram(new CShader(vs, CShader::E_VERTEX), new CShader(fs, CShader::E_FRAGMENT))
{

}

ScreenShader::~ScreenShader()
{

}
