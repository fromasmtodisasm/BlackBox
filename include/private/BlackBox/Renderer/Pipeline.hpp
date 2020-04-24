#pragma once
#include <glm/glm.hpp>
#include <BlackBox/Renderer/BaseTexture.hpp>
#include <BlackBox/Renderer/Shader.hpp>

class Object;
namespace Pipeline
{
    IShaderProgram* bindProgram(const char* name);
}
