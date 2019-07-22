#pragma once
#include <BlackBox/CShader.hpp>
#include <string>

struct IShaderManager
{
  virtual CBaseShaderProgram *getShader(std::string name) = 0;
};
