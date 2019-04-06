#pragma once
#include "CShader.hpp"
#include <string>

struct IShaderManager
{
  virtual CShaderProgram *getShader(std::string name) = 0;
};
