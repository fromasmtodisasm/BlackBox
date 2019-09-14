#include <BlackBox/Render/ShadowMapShader.hpp>

const char* ShadowMapShader::vs = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
)";

const char* ShadowMapShader::fs = R"(
#version 330 core

void main()
{
    //gl_FragDepth = gl_FragCoord.z;
}
)";

ShadowMapShader::ShadowMapShader() :
  m_Engine(GetISystem()), 
	CBaseShaderProgram(CShader::loadFromMemory(vs, CShader::E_VERTEX), CShader::loadFromMemory(fs, CShader::E_FRAGMENT))
{
	
}

ShadowMapShader::~ShadowMapShader()
{
}

void ShadowMapShader::setup()
{
}

