#pragma once
#include <BlackBox/Shader.hpp>
#include <string>

struct IShaderManager
{
  virtual CBaseShaderProgram *getShader(std::string name) = 0;
};
