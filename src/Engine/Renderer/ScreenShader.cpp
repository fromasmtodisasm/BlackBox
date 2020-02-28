#include <BlackBox/Renderer/ScreenShader.hpp>

const char* vs = R"(
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
const char* fs = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
    //FragColor = texture(screenTexture, TexCoords);
		FragColor = vec4(1.0, 1.f, 0.f, 1.f);
}
)";

ScreenShader::ScreenShader()
  :
  CBaseShaderProgram(CShader::loadFromMemory(vs, CShader::E_VERTEX), CShader::loadFromMemory(fs, CShader::E_FRAGMENT))
{
  //CBaseShaderProgram::create();
}

ScreenShader::~ScreenShader()
{
}