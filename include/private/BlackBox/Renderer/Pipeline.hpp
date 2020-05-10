#pragma once
#include <glm/glm.hpp>
#include <BlackBox/Renderer/BaseTexture.hpp>
#include <BlackBox/Renderer/IShader.hpp>

class CStatObj;
namespace Pipeline
{
    IShaderProgram* bindProgram(const char* name);
}
