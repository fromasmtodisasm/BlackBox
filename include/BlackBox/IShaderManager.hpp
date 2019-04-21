#pragma once
#include <BlackBox/CShader.hpp>
#include <string>

struct IShaderManager
{
  virtual CShaderProgram *getShader(std::string name) = 0;
};
